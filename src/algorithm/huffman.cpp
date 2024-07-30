// TODO: more constexprs, noexcepts and consts.
// TODO: try to avoid errors, for example, with invalid dump received.

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
#include "core/decompressor.h"
#include "core/freq_counter.h"

namespace algorithm {

// Note that the buffer size must be large enough to contain a tree dump.
constexpr size_t INPUT_BUF_SIZE{16384};

// Note that the buffer size must be large enough to contain a tree dump.
constexpr size_t OUTPUT_BUF_SIZE{16384};

// The direction of the tree path.
enum class TreeDirection {
	Left,
	Right,
};

// Path of the tree.
using TreePath = std::vector<TreeDirection>;

// Tree path of each character.
using TreePaths = std::map<char, TreePath>;

// Huffman compression tree node to store characters with corresponsing
// frequency and left and right branches.
class TreeNode {
	public:
		// Constructs empty node.
		constexpr TreeNode() noexcept = default;

		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr TreeNode(char ch, uint64_t freq) noexcept;

		// Creates a grouping node with left and right branches, which both must not
		// be `nullptr`.
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
		//
		// Make sure that buffer is big enough to store node's dump.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

		// Returns the character of the node.
		constexpr char get_ch() const noexcept;

		// Returns the left branch of the node.
		constexpr const TreeNode* get_left() const noexcept;

		// Returns the right branch of the node.
		constexpr const TreeNode* get_right() const noexcept;

		// Determines that node is another nodes group.
		constexpr bool is_group() const noexcept;

		// Loads a new node from the passed buffer starting from passed bit index.
		// Note that this method does not load frequencies, since the dump does not
		// contain them. Frequencies are just needed to build the tree.
		//
		// Destructs and deallocates left and right branches if exists.
		constexpr void load(const char* buf, size_t& bit_index) noexcept;

	private:
		// A character with a certain frequency or a random character if the left and
		// right branches are set.
		char ch_{'\0'};

		// Character frequency or the sum of the frequencies of the left and right
		// branches. Frequency can be zero only when loading a dump, since the
		// frequency is only needed to build a tree.
		uint64_t freq_{0};

		// They are either both `nullptr`, or they both point to other nodes.
		TreeNode* left_{nullptr};
		TreeNode* right_{nullptr};
};

// Huffman compression characters tree.
class Tree {
	public:
		// Constructs empty tree.
		constexpr Tree() noexcept = default;

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

		// Dumps the tree to the passed buffer starting from passed bit index. Note
		// that this method does not dump frequencies because they are only needed to
		// build the tree.
		//
		// Make sure that buffer is big enough to store node's dump.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

		// Returns the root of the tree.
		constexpr const TreeNode* get_root() const noexcept;

		// Loads the tree from the passed buffer starting from passed bit index.
		//
		// Destructs and deallocates current tree if exists.
		constexpr void load(const char* buf, size_t& bit_index) noexcept;

	private:
		TreeNode* root_{nullptr};
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
	if (!input.seekg(0, std::ios::beg)) {
		throw core::CompressorError{"failed to seek to the beginning of the input"};
	}
	// Write readed input size to the output.
	output << freq_counter.get_total();
	if (output.bad()) {
		throw core::CompressorError{"failed to write input's size to the output"};
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
	TreePaths paths = tree.calculate_paths();

	// Create buffer to store readed input content.
	char input_buf[INPUT_BUF_SIZE]{};

	// Read, compress and write input content.
	while (!input.eof()) {
		// Read input to the buffer.
		input.read(input_buf, sizeof(input_buf));
		if (input.bad() || (input.fail() && !input.eof())) {
			throw core::CompressorError{"failed to read from input stream"};
		}

		// Compress and write each readed character.
		for (auto char_index{0}; char_index < input.gcount(); ++char_index) {
			// Get character's tree path.
			const TreePath& path = paths[input_buf[char_index]];

			// Write path bits to the output buffer.
			for (TreeDirection direction : path) {
				// Dump direction's bit.
				huffman_tree_direction_dump(direction, output_buf, output_buf_bit_index);

				// Do not drain the output buffer if it is not full.
				if (output_buf_bit_index / CHAR_BIT < sizeof(output_buf)) {
					continue;
				}

				// Drain the output buffer to the stream.
				if (output.write(output_buf, sizeof(output_buf)).bad()) {
					throw core::CompressorError{
						"failed to write the part of compressed content"};
				}
				std::ranges::fill(output_buf, 0);
				output_buf_bit_index = 0;
			}
		}
	}

	// Try to write the leftover content of the output buffer.
	if (output_buf_bit_index > 0) {
		const size_t output_size{
			output_buf_bit_index / CHAR_BIT + (output_buf_bit_index % CHAR_BIT > 0)};
		if (output.write(output_buf, output_size).bad()) {
			throw core::CompressorError{
				"failed to write the last part of compressed content"};
		}
	}

	// Try to flush the output.
	if (output.flush().bad()) {
		throw core::CompressorError{"failed to flush the output"};
	}
}

// TODO: Make more readable, split into small functions.
void HuffmanDecompressor::decompress(
		std::istream& input, std::ostream& output) {
	// Read decompressed content size.
	uint64_t decompressed_size;
	input >> decompressed_size;
	if (input.fail()) {
		throw core::DecompressorError{"failed to read decompressed content size"};
	}

	// Create input buffer to store tree dump and compressed content.
	char input_buf[INPUT_BUF_SIZE]{};
	size_t input_buf_bit_index{0};

	// Read the first chunk, which contains a tree dump and some compressed
	// content.
	input.read(input_buf, sizeof(input_buf));
	if (input.bad() || (input.fail() && !input.eof())) {
		throw core::CompressorError{"failed to read from input stream"};
	}

	// Load the tree using dump from readed chunk.
	Tree tree;
	tree.load(input_buf, input_buf_bit_index);

	// Create output buffer to store decompressed content.
	char output_buf[OUTPUT_BUF_SIZE]{};
	size_t output_buf_index{0};

	// Traverses the tree according to the path bits in the compressed content
	// until it encounters a symbol in the tree. The found symbol writes to the
	// output buffer.
	for (size_t i{0}; i < decompressed_size; ++i) {
		const TreeNode* dip_node = tree.get_root();

		// Traverse the tree according to the path bits.
		while (dip_node->is_group()) {
			// Read the path bit. If it is equal to 1, then select the right branch,
			// otherwise - the left.
			dip_node = core::bit_get(input_buf, input_buf_bit_index) == 0b1
				? dip_node->get_right() : dip_node->get_left();
			++input_buf_bit_index;

			// Do not read the next chunk of input if the input buffer has not run out.
			if (input_buf_bit_index / CHAR_BIT < sizeof(input_buf)) {
				continue;
			}

			// Read the next chunk to the input buffer.
			input.read(input_buf, sizeof(input_buf));
			if (input.bad() || (input.fail() && !input.eof())) {
				throw core::CompressorError{"failed to read the next input chunk"};
			}
			input_buf_bit_index = 0;
		}

		// Write character to the output buffer.
		output_buf[output_buf_index++] = dip_node->get_ch();

		// Do not drain the output buffer if it is not full.
		if (output_buf_index < sizeof(output_buf)) {
			continue;
		}

		// Drain the output buffer to the stream.
		if (output.write(output_buf, sizeof(output_buf)).bad()) {
			throw core::CompressorError{
				"failed to write the part of decompressed content"};
		}
		std::ranges::fill(output_buf, 0);
		output_buf_index = 0;
	}

	// Try to write the leftover content of the output buffer.
	if (output_buf_index > 0
			&& output.write(output_buf, output_buf_index).bad()) {
		throw core::CompressorError{
			"failed to write the last part of compressed content"};
	}

	// Try to flush the output.
	if (output.flush().bad()) {
		throw core::CompressorError{"failed to flush the output"};
	}
}

constexpr TreeNode::TreeNode(const char ch, const uint64_t freq) noexcept
		: ch_{ch}, freq_{freq} {
	assert(freq > 0 && "zero frequency is useless for the tree building");
}

constexpr TreeNode::TreeNode(
		TreeNode* const left, TreeNode* const right) noexcept
		: left_{left}, right_{right} {
	// A grouping node must contain both left and right branches.
	assert(nullptr != left && nullptr != right && "they must not be `nullptr`");

	// Update frequency after branch pointers validation.
	freq_ = left->freq_ + right->freq_;
	assert(freq_ > 0 && "zero frequency is useless for the tree building");
}

constexpr TreeNode::~TreeNode() noexcept {
	// Deallocate branches or do nothing if they are `nullptr`.
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

constexpr char TreeNode::get_ch() const noexcept {
	return ch_;
}

constexpr const TreeNode* TreeNode::get_left() const noexcept {
	return left_;
}

constexpr const TreeNode* TreeNode::get_right() const noexcept {
	return right_;
}

constexpr bool TreeNode::is_group() const noexcept {
	// There is no need to check right node against `nullptr` because a grouping
	// node has either 0 branches or both.
	return nullptr != left_;
}

constexpr void TreeNode::load(
		const char* const buf, size_t& bit_index) noexcept {
	// Destruct and deallocate left and right branches of old node.
	delete left_;
	delete right_;
	// Zeroize frequency because node dump does not contain frequencies.
	freq_ = 0;

	// Check that we need to load left and right branches or character.
	if (core::bit_get(buf, bit_index) == 0b1) {
		// Shift bit index to load the left branch.
		++bit_index;

		// Allocate and load the left branch of the node.
		left_ = new TreeNode;
		left_->load(buf, bit_index);

		// Allocate and load the right branch of the node.
		right_ = new TreeNode;
		right_->load(buf, bit_index);
	} else {
		// Shift bit index to load the character.
		++bit_index;

		// Set `nullptr` to branches because new node is character node.
		left_ = nullptr;
		right_ = nullptr;
		// Read character and shift by readed bits.
		ch_ = core::bit_read_char(buf, bit_index);
		bit_index += CHAR_BIT;
	}
}

Tree::Tree(const core::FreqCounter& freq_counter) noexcept: root_{nullptr} {
	// Create character nodes with corresponding frequencies greater than 0.
	std::vector<TreeNode*> nodes;
	for (const auto& [ch, count] : freq_counter) {
		nodes.push_back(new TreeNode{ch, count});
	}
	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes by frequency descending.
		std::ranges::sort(nodes, core::LessPtr<TreeNode>{});

		// Group the last two elements into a grouping node.
		TreeNode* const right = nodes.back();
		nodes.pop_back();
		TreeNode* const left = nodes.back();
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

constexpr void Tree::dump(char* const buf, size_t& bit_index) const noexcept {
	if (nullptr != root_) {
		root_->dump(buf, bit_index);
	}
}

constexpr const TreeNode* Tree::get_root() const noexcept {
	return root_;
}

constexpr void Tree::load(const char* const buf, size_t& bit_index) noexcept {
	// Destruct and deallocate current tree to store a new tree.
	delete root_;

	// Allocate and load a new root node from accepted buffer.
	root_ = new TreeNode;
	root_->load(buf, bit_index);
}

}  // namespace algorithm
