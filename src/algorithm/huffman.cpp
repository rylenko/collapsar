#include "huffman.h"

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <format>
#include <fstream>
#include <ios>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "core/comparator.h"
#include "core/compressor.h"
#include "core/freq_counter.h"

namespace algorithm {

class Node {
	public:
		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr Node(unsigned char ch, size_t freq) noexcept;

		// Creates a grouping node with left and right branches. In the case of this
		// constructor both cannot be `nullptr`, you need to use another constructor.
		constexpr Node(const Node* left, const Node* right) noexcept;

		// A node is less than another if its frequency is higher. That is, the most
		// frequent node requires less weight.
		friend constexpr bool operator<(const Node& x, const Node& y) noexcept;

	private:
		// A character with a certain frequency or a random character if the left and
		// right branches are set.
		unsigned char ch_;
		// Character frequency or the sum of the frequencies of the left and right
		// branches. Must be greater than 0.
		size_t freq_;

		// They are either both `nullptr`, or they both point to other nodes.
		const Node* left_;
		const Node* right_;
};

class Tree {
	public:
		constexpr explicit Tree(const Node* root) noexcept;
		constexpr explicit Tree(const core::FreqCounter& freq_counter) noexcept;

	private:
		const Node* root_;
};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Huffman compression requires double read. So to seek later we need to
	// require input to be ifstream.
	validate_input_is_ifstream_(input);
	// Enable exceptions for the input stream and save original exception mask.
	auto input_original_exceptions =
		core::Compressor::set_ios_fail_and_bad_exceptions_(input);

	// Frequencies counter of input stream characters.
	core::FreqCounter freq_counter;
	// Count frequencies of characters.
	try {
		input >> freq_counter;
	} catch (const std::ios_base::failure& e) {
		// Throw an error if eofbit was not set.
		if (!input.eof()) {
			throw core::CompressorError(std::format(
				"failed to count frequencies: {}", e.what()));
		}
		// Clear EOF state (eofbit and failbit) to seek to the beginning of the
		// stream later.
		input.clear();
	}

	// Build a tree using counted frequencies.
	const Tree tree{freq_counter};

	// Seek to the beginning of the input to read again and compress characters
	// using built tree path codes.
	try {
		input.seekg(0, std::ios::beg);
	} catch (const std::ios_base::failure& e) {
		throw core::CompressorError(std::format(
			"failed to seek to the beginning of the input stream: {}", e.what()));
	}

	output << "This is Huffman decompressor." << std::endl;

	// TODO: core::Compressor::set_stream_exceptions.
	// Try to restore input's original exception mask.
	try {
		input.exceptions(input_original_exceptions);
	} catch (const std::ios_base::failure& e) {
		throw core::CompressorError(std::format(
			"failed to restore exceptions for input stream: {}", e.what()));
	}
}

void HuffmanCompressor::validate_input_is_ifstream_(
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

constexpr Node::Node(const unsigned char ch, const size_t freq) noexcept
		: ch_{ch}, freq_{freq}, left_{nullptr}, right_{nullptr} {
	assert(freq > 0 && "zero frequency is useless for the tree");
}

constexpr Node::Node(const Node* const left, const Node* const right) noexcept
		: ch_{'\0'},
		freq_{0},  // Will change in the constructor body after pointers validation.
		left_{left},
		right_{right} {
	// A grouping node must contain at least one branch to obtain a non-zero
	// frequency.
	assert(nullptr != left && nullptr != right && "they must not be `nullptr`");
	// Update frequency after branch pointers validation.
	freq_ = left->freq_ + right->freq_;
}

constexpr bool operator<(const Node& x, const Node& y) noexcept {
	return x.freq_ > y.freq_;
}

constexpr Tree::Tree(const Node* const root) noexcept: root_{root} {}

constexpr Tree::Tree(
		const core::FreqCounter& freq_counter) noexcept: Tree{nullptr} {
	// Character nodes to store frequencies greater than 0.
	std::vector<const Node*> nodes;

	// Create character nodes with corresponding frequencies greater than 0.
	unsigned char ch{'\0'};
	do {
		if (freq_counter[ch] > 0) {
			nodes.push_back(new Node(ch, freq_counter[ch]));
		}
	} while (0 != ++ch);

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes. See less operator implementation for more.
		std::sort(nodes.begin(), nodes.end(), core::LessPtr<Node>{});

		// Group the last two elements into a grouping node.
		const Node* const right{nodes.back()};
		nodes.pop_back();
		const Node* const left{nodes.back()};
		nodes.pop_back();
		nodes.push_back(new Node{left, right});
	}

	// Replace current `nullptr` root with built root.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

}  // namespace algorithm
