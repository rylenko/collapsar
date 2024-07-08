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

inline constexpr size_t BUF_SIZE{4096};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Huffman compression requires double read. So to seek later we need to
	// require input to be ifstream.
	validate_input_is_ifstream_(input);

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

	// Build the tree using character frequencies.
	HuffmanTree tree{freq_counter};
	compress_(input, tree, output);
}

void HuffmanCompressor::compress_(
		std::istream& input,
		const HuffmanTree& tree,
		std::ostream& output) noexcept {
	(void)input;
	char buf[BUF_SIZE]{};
	size_t buf_bit_index{0};

	// Dump the tree to the buffer.
	tree.dump(buf, buf_bit_index);

	// Get each character tree paths.
	HuffmanTreePaths paths{tree.calculate_paths()};

	output << "This is Huffman decompressor." << std::endl;
}

constexpr void HuffmanCompressor::validate_input_is_ifstream_(
		const std::istream& input) {
	if (!dynamic_cast<const std::ifstream*>(&input)) {
		throw core::CompressorError(
			"Huffman compression requires double reads using seek, so only file streams"
			" are supported");
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
	// Destruct and deallocate branches if current node is grouping node.
	if (is_group()) {
		left_->~HuffmanTreeNode();
		delete left_;

		left_->~HuffmanTreeNode();
		delete left_;
	}
}

constexpr bool operator<(
		const HuffmanTreeNode& x, const HuffmanTreeNode& y) noexcept {
	return x.freq_ > y.freq_;
}

constexpr void HuffmanTreeNode::calculate_paths(
		HuffmanTreePaths& paths,
		HuffmanTreePath& buf,
		size_t buf_index) const noexcept {
	if (is_group()) {
		// Continue recursion through left branch from current buffer position.
		buf[buf_index] = HuffmanTreeDirection::Left;
		left_->calculate_paths(paths, buf, buf_index + 1);

		// Continue recursion through left branch from current buffer position.
		buf[buf_index] = HuffmanTreeDirection::Right;
		right_->calculate_paths(paths, buf, buf_index + 1);
	} else {
		// Copy current buffer to the paths map.
		auto inserter{std::back_inserter(paths[ch_])};
		std::ranges::copy_n(buf.begin(), buf_index + 1, inserter);
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
		core::bit_write(buf, bit_index, ch_);
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
	for (auto it{freq_counter.begin()}; freq_counter.end() != it; ++it) {
		nodes.push_back(new HuffmanTreeNode(it->first, it->second));
	}

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes by frequency descending.
		std::sort(nodes.begin(), nodes.end(), core::LessPtr<HuffmanTreeNode>{});

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
	if (nullptr != root_) {
		// Call node's destructor to free possible left and right subnodes.
		root_->~HuffmanTreeNode();
		// Free allocated node.
		delete root_;
	}
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
