#include "algorithm/huffman.h"

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

// Huffman compression tree node to store characters with corresponsing
// frequency or left and right nodes.
class Node {
	public:
		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr Node(unsigned char ch, size_t freq) noexcept;

		// Creates a grouping node with left and right branches. In the case of this
		// constructor both cannot be `nullptr`, you need to use another constructor.
		constexpr Node(Node* left, Node* right) noexcept;

		// TODO(rylenko): ~Node();

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
		Node* left_;
		Node* right_;
};

// Huffman compression characters tree.
class Tree {
	public:
		// Counts the frequencies of content from the `input`, then builds a tree,
		// where the more frequent characters are closer to the root.
		//
		// After successfully construction, the stream must be in the EOF state:
		// eofbit and failbit.
		explicit Tree(std::istream& input);

		// TODO(rylenko): ~Tree();

	private:
		Node* root_;
};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Huffman compression requires double read. So to seek later we need to
	// require input to be ifstream.
	validate_input_is_ifstream_(input);

	// Build the tree of input data.
	Tree tree{input};
	// Clear EOF state (eofbit and failbit) to seek to the beginning of the
	// stream.
	input.clear();
	// Seek to the beginning of the input to give the ability to read content
	// again later.
	input.seekg(0, std::ios::beg);
	if (!input.good()) {
		throw core::CompressorError("failed to seek to the beginning of the input");
	}

	output << "This is Huffman decompressor." << std::endl;
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

constexpr Node::Node(Node* const left, Node* const right) noexcept
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

Tree::Tree(std::istream& input): root_{nullptr} {
	// Count frequencies of characters.
	core::FreqCounter freq_counter;
	try {
		input >> freq_counter;
	} catch (const core::FreqCounterError& e) {
		throw core::CompressorError(
			std::format("failed to count frequencies: {}", e.what()));
	}

	// Character nodes to store frequencies greater than 0.
	std::vector<Node*> nodes;
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
		Node* const right{nodes.back()};
		nodes.pop_back();
		Node* const left{nodes.back()};
		nodes.pop_back();
		nodes.push_back(new Node{left, right});
	}
	// Replace current `nullptr` root with built root.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

}  // namespace algorithm
