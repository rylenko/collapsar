// TODO: more constexprs, noexcepts and consts

#include "algorithm/huffman.h"

#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <fstream>
#include <ios>
#include <istream>
#include <iterator>
#include <map>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "core/bit.h"
#include "core/comparator.h"
#include "core/compressor.h"
#include "core/freq_counter.h"

namespace algorithm {

constexpr size_t INPUT_BUF_SIZE{8192};
constexpr size_t OUTPUT_BUF_SIZE{8192};

// The direction of the path, which can be represented as an integer. Left
// means 0 and right means 1.
enum class TreeDirection {
	Left,
	Right,
};

// Path of the tree.
using TreePath = std::vector<TreeDirection>;

// Tree path of each character.
using TreePaths = std::map<char, TreePath>;

// Huffman compression tree node to store characters with corresponsing
// frequency or left and right nodes.
class TreeNode {
	public:
		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr TreeNode(char ch, uint64_t freq) noexcept;

		// Creates a grouping node with left and right branches. In the case of this
		// constructor both cannot be `nullptr`, you need to use another constructor.
		constexpr TreeNode(TreeNode* left, TreeNode* right) noexcept;

		// Destructs the node.
		constexpr ~TreeNode() noexcept;

		// A node is less than another if its frequency is higher. That is, the most
		// frequent node requires less weight.
		friend constexpr bool operator<(
			const TreeNode& x, const TreeNode& y) noexcept;

		// Recursively calculates character node paths using buffer and its position.
		constexpr void calculate_paths(
			TreePaths& paths, TreePath& buf, size_t buf_index) const noexcept;

		// Dumps current node to the passed buffer starting from passed bit index.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

		// Determines that node is another nodes group.
		constexpr bool is_group() const noexcept;

	private:
		// A character with a certain frequency or a random character if the left and
		// right branches are set.
		char ch_;
		// Character frequency or the sum of the frequencies of the left and right
		// branches. Must be greater than 0.
		uint64_t freq_;

		// They are either both `nullptr`, or they both point to other nodes.
		TreeNode* left_;
		TreeNode* right_;
};

// Huffman compression characters tree.
class Tree {
	public:
		// Counts the frequencies of content from the `input`, then builds a tree,
		// where the more frequent characters are closer to the root.
		//
		// After successfully construction, the stream must be in the EOF state:
		// eofbit and failbit.
		explicit Tree(const core::FreqCounter& freq_counter) noexcept;

		// Destructs the built tree.
		constexpr ~Tree() noexcept;

		// Calculate paths. So this function calculates tree path for each character.
		TreePaths calculate_paths() const noexcept;

		// Dumps the tree to the passed buffer starting from passed bit index.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

	private:
		TreeNode* root_;
};

// Dumps the tree direction bit to the passed buffer at the passed bit index.
// Then it increments the passed bit index.
constexpr void huffman_tree_direction_dump(
		TreeDirection direction, char* const buf, size_t& bit_index) noexcept {
	switch (direction) {
	case TreeDirection::Left:
		core::bit_clear(buf, bit_index);
		break;
	case TreeDirection::Right:
		core::bit_set(buf, bit_index);
		break;
	}
	++bit_index;
}


// TODO: Make more readable, split into small functions.
void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Count frequencies of characters to build a tree with optimal paths.
	core::FreqCounter freq_counter;
	input >> freq_counter;
	// Clear eofbit and failbit to seek to the beginning of the stream.
	input.clear();
	// Seek to the beginning of the input to read content again and apply tree
	// paths to it.
	input.seekg(0, std::ios::beg);
	if (!input) {
		throw core::CompressorError("failed to seek to the beginning of the input");
	}
	// Write readed input size to the output.
	output << freq_counter.get_total();
	if (output.bad()) {
		throw core::CompressorError("failed to write input's size to the output");
	}

	// Create output buffer to store tree dump and compressed content.
	char output_buf[OUTPUT_BUF_SIZE]{};
	size_t output_buf_bit_index{0};

	// Build the tree using character frequencies. We will encode characters using
	// tree paths.
	Tree tree{freq_counter};
	// Dump the tree to the buffer to restore it during decompression.
	tree.dump(output_buf, output_buf_bit_index);

	// Get the tree paths of each character to compress these character.
	TreePaths paths{tree.calculate_paths()};

	// Create buffer to store readed input content.
	char input_buf[INPUT_BUF_SIZE]{};

	// Read, compress and write input content.
	while (!input.eof()) {
		// Read input to the buffer.
		input.read(input_buf, sizeof(input_buf));
		if (input.bad() || (input.fail() && !input.eof())) {
			throw core::CompressorError("failed to read from input stream");
		}

		// Compress and write each readed character.
		for (auto char_index{0}; char_index < input.gcount(); ++char_index) {
			// Get character's tree path.
			const TreePath& path{paths[input_buf[char_index]]};

			// Write path bits to the output buffer.
			for (TreeDirection direction : path) {
				// Dump direction's bit.
				huffman_tree_direction_dump(direction, output_buf, output_buf_bit_index);

				// Do not drain the output buffer if it is not full.
				if (output_buf_bit_index / CHAR_BIT < sizeof(output_buf)) {
					continue;
				}

				// Drain the output buffer to the stream.
				output.write(output_buf, sizeof(output_buf));
				if (output.bad()) {
					throw core::CompressorError(
						"failed to write the part of compressed content");
				}
				std::ranges::fill(output_buf, 0);
				output_buf_bit_index = 0;
			}
		}
	}

	// Try to write the leftover content of the output buffer.
	if (output_buf_bit_index > 0) {
		const size_t output_size =
			output_buf_bit_index / CHAR_BIT + (output_buf_bit_index % CHAR_BIT > 0);
		output.write(output_buf, output_size);
		if (output.bad()) {
			throw core::CompressorError(
				"failed to write the last part of compressed content");
		}
	}

	// Try to flush the output.
	output.flush();
	if (output.bad()) {
		throw core::CompressorError("failed to flush the output.");
	}
}

void HuffmanDecompressor::decompress(
		std::istream& input, std::ostream& output) {
	(void)input;
	output << "This is Huffman decompressor." << std::endl;
}

constexpr TreeNode::TreeNode(const char ch, const uint64_t freq) noexcept
		: ch_{ch}, freq_{freq}, left_{nullptr}, right_{nullptr} {
	assert(freq > 0 && "zero frequency is useless for the tree");
}

constexpr TreeNode::TreeNode(
		TreeNode* const left, TreeNode* const right) noexcept
		: ch_{'\0'},
		freq_{0},  // Will change in the constructor body after pointers validation.
		left_{left},
		right_{right} {
	// A grouping node must contain at least one branch to obtain a non-zero
	// frequency.
	assert(nullptr != left && nullptr != right && "they must not be `nullptr`");

	// Update frequency after branch pointers validation.
	freq_ = left->freq_ + right->freq_;
	assert(freq_ > 0 && "zero frequency is useless for the tree");
}

constexpr TreeNode::~TreeNode() noexcept {
	// Deallocate branches or do nothing if they are nullptr.
	delete left_;
	delete right_;
}

constexpr bool operator<(const TreeNode& x, const TreeNode& y) noexcept {
	return x.freq_ > y.freq_;
}

constexpr void TreeNode::calculate_paths(
		TreePaths& paths, TreePath& buf, const size_t buf_index) const noexcept {
	if (is_group()) {
		// Continue recursion through left branch from current buffer position.
		if (buf_index >= buf.size()) {
			buf.push_back(TreeDirection::Left);
		} else {
			buf[buf_index] = TreeDirection::Left;
		}
		left_->calculate_paths(paths, buf, buf_index + 1);

		// Continue recursion through left branch from current buffer position.
		//
		// The buffer size is sufficient here because we expanded it earlier to the
		// current level.
		buf[buf_index] = TreeDirection::Right;
		right_->calculate_paths(paths, buf, buf_index + 1);
	} else {
		// Copy current buffer to the paths map.
		std::copy_n(buf.begin(), buf_index, std::back_inserter(paths[ch_]));
	}
}

// Dumps current node to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void TreeNode::dump(
		char* const buf, size_t& bit_index) const noexcept {
	if (is_group()) {
		// Set the bit, which means that we have a grouping node.
		core::bit_set(buf, bit_index);
		++bit_index;

		// Dump left and right branches.
		left_->dump(buf, bit_index);
		right_->dump(buf, bit_index);
	} else {
		// Clear the bit, which means that we have a character node.
		core::bit_clear(buf, bit_index);
		++bit_index;

		// Write character to the buffer.
		core::bit_write_char(buf, bit_index, ch_);
		bit_index += CHAR_BIT;
	}
}

constexpr bool TreeNode::is_group() const noexcept {
	// There is no need to check right node against `nullptr` because a grouping
	// node has either 0 branches or both.
	return nullptr != left_;
}

Tree::Tree(const core::FreqCounter& freq_counter) noexcept: root_{nullptr} {
	// Character nodes to store frequencies greater than 0.
	std::vector<TreeNode*> nodes;
	// Create character nodes with corresponding frequencies greater than 0.
	for (const auto& [ch, count] : freq_counter) {
		nodes.push_back(new TreeNode{ch, count});
	}

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes by frequency descending.
		std::ranges::sort(nodes, core::LessPtr<TreeNode>{});

		// Group the last two elements into a grouping node.
		TreeNode* const right{nodes.back()};
		nodes.pop_back();
		TreeNode* const left{nodes.back()};
		nodes.pop_back();
		nodes.push_back(new TreeNode{left, right});
	}
	// Replace current `nullptr` root with built root if exists.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

constexpr Tree::~Tree() noexcept {
	// Free allocated root node or do nothing if it is nullptr.
	delete root_;
}

TreePaths Tree::calculate_paths() const noexcept {
	TreePaths paths;
	if (nullptr != root_) {
		// Calculate paths recursively using buffer from the root node.
		TreePath buf;
		root_->calculate_paths(paths, buf, 0);
	}
	return paths;
}

// Dumps the tree to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void Tree::dump(char* const buf, size_t& bit_index) const noexcept {
	if (nullptr != root_) {
		root_->dump(buf, bit_index);
	}
}

}  // namespace algorithm
