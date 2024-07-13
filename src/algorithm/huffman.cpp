// TODO: more constexprs, noexcepts and consts

#include "algorithm/huffman.h"

#include <cassert>
#include <climits>
#include <cstddef>

#include <algorithm>
#include <fstream>
#include <ios>
#include <istream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "core/bit.h"
#include "core/comparator.h"
#include "core/compressor.h"
#include "core/freq_counter.h"

namespace algorithm {

inline constexpr size_t INPUT_BUF_SIZE{8192};
inline constexpr size_t OUTPUT_BUF_SIZE{8192};

// Dumps the tree direction to the passed buffer starting from passed bit
// index.
constexpr void huffman_tree_direction_dump(
		HuffmanTreeDirection direction,
		char* const buf,
		size_t& bit_index) noexcept {
	// Choose bit 0 or bit 1 and write it.
	switch (direction) {
	case HuffmanTreeDirection::Left:
		core::bit_clear(buf, bit_index);
		break;
	case HuffmanTreeDirection::Right:
		core::bit_set(buf, bit_index);
		break;
	}
	++bit_index;
}


void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Count frequencies of characters to build a tree with optimal paths.
	core::FreqCounter freq_counter;
	input >> freq_counter;
	// Clear eofbit and failbit to seek to the beginning of the stream.
	input.clear();
	// Seek to the beginning of the input to apply tree paths.
	input.seekg(0, std::ios::beg);
	if (!input.good()) {
		throw core::CompressorError("failed to seek to the beginning of the input");
	}
	// Write readed input size to the output.
	output << freq_counter.get_total();
	if (output.bad()) {
		throw core::CompressorError("failed to write input's size to the output");
	}

	// Build the tree using character frequencies.
	HuffmanTree tree{freq_counter};
	// Compress input's content to output through built tree.
	compress_(input, tree, output);
}

// TODO: Make more readable, split into small functions.
void HuffmanCompressor::compress_(
		std::istream& input, const HuffmanTree& tree, std::ostream& output) {
	// Create buffer to read input content.
	char input_buf[INPUT_BUF_SIZE]{};

	// Create buffer to conveniently work with bits.
	char output_buf[OUTPUT_BUF_SIZE]{};
	size_t output_buf_bit_index{0};

	// Dump the tree to the buffer.
	tree.dump(output_buf, output_buf_bit_index);

	// Get each character tree paths.
	HuffmanTreePaths paths{tree.calculate_paths()};

	// Process input's content.
	while (!input.eof()) {
		// Read input to the buffer.
		input.read(input_buf, sizeof(input_buf));
		if (!input && !input.eof()) {
			throw core::CompressorError("failed to read from input stream");
		}

		// Compress each readed character.
		for (auto char_index{0}; char_index < input.gcount(); ++char_index) {
			// Get character's tree path.
			const HuffmanTreePath& path{paths[input_buf[char_index]]};

			// Write path bits to the output buffer.
			for (HuffmanTreeDirection direction : path) {
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

	// Write the leftover content of the output buffer.
	if (output_buf_bit_index > 0) {
		const size_t tail = output_buf_bit_index % CHAR_BIT > 0 ? 1 : 0;
		output.write(output_buf, output_buf_bit_index / CHAR_BIT + tail);
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

constexpr HuffmanTreeNode::HuffmanTreeNode(
		const char ch, const size_t freq) noexcept
		: ch_{ch}, freq_{freq}, left_{nullptr}, right_{nullptr} {
	assert(freq > 0 && "zero frequency is useless for the tree");
}

constexpr HuffmanTreeNode::HuffmanTreeNode(
		HuffmanTreeNode* const left, HuffmanTreeNode* const right) noexcept
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

constexpr HuffmanTreeNode::~HuffmanTreeNode() noexcept {
	// Deallocate branches or do nothing if they are nullptr.
	delete left_;
	delete right_;
}

constexpr bool operator<(
		const HuffmanTreeNode& x, const HuffmanTreeNode& y) noexcept {
	return x.freq_ > y.freq_;
}

constexpr void HuffmanTreeNode::calculate_paths(
		HuffmanTreePaths& paths,
		HuffmanTreePath& buf,
		const size_t buf_index) const noexcept {
	if (is_group()) {
		// Continue recursion through left branch from current buffer position.
		if (buf_index >= buf.size()) {
			buf.push_back(HuffmanTreeDirection::Left);
		} else {
			buf[buf_index] = HuffmanTreeDirection::Left;
		}
		left_->calculate_paths(paths, buf, buf_index + 1);

		// Continue recursion through left branch from current buffer position.
		//
		// The buffer size is sufficient here because we expanded it earlier to the
		// current level.
		buf[buf_index] = HuffmanTreeDirection::Right;
		right_->calculate_paths(paths, buf, buf_index + 1);
	} else {
		// Copy current buffer to the paths map.
		std::copy_n(buf.begin(), buf_index, std::back_inserter(paths[ch_]));
	}
}

// Dumps current node to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void HuffmanTreeNode::dump(
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

constexpr bool HuffmanTreeNode::is_group() const noexcept {
	// There is no need to check right node against `nullptr` because a grouping
	// node has either 0 branches or both.
	return nullptr != left_;
}

HuffmanTree::HuffmanTree(
		const core::FreqCounter& freq_counter) noexcept: root_{nullptr} {
	// Character nodes to store frequencies greater than 0.
	std::vector<HuffmanTreeNode*> nodes;
	// Create character nodes with corresponding frequencies greater than 0.
	for (const auto& [ch, count] : freq_counter) {
		nodes.push_back(new HuffmanTreeNode{ch, count});
	}

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes by frequency descending.
		std::ranges::sort(nodes, core::LessPtr<HuffmanTreeNode>{});

		// Group the last two elements into a grouping node.
		HuffmanTreeNode* const right{nodes.back()};
		nodes.pop_back();
		HuffmanTreeNode* const left{nodes.back()};
		nodes.pop_back();
		nodes.push_back(new HuffmanTreeNode{left, right});
	}
	// Replace current `nullptr` root with built root if exists.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

constexpr HuffmanTree::~HuffmanTree() noexcept {
	// Free allocated root node or do nothing if it is nullptr.
	delete root_;
}


HuffmanTreePaths HuffmanTree::calculate_paths() const noexcept {
	HuffmanTreePaths paths;
	if (nullptr != root_) {
		// Calculate paths recursively using buffer from the root node.
		HuffmanTreePath buf;
		root_->calculate_paths(paths, buf, 0);
	}
	return paths;
}

// Dumps the tree to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void HuffmanTree::dump(
		char* const buf, size_t& bit_index) const noexcept {
	if (nullptr != root_) {
		root_->dump(buf, bit_index);
	}
}

}  // namespace algorithm
