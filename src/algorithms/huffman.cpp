#include "huffman.h"

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <istream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "../core/freq_counter.h"

namespace algorithms {

class Node {
	public:
		constexpr Node(unsigned char ch, size_t freq);
		constexpr Node(
			unsigned char ch, size_t freq, const Node* left, const Node* right);

		// A node is less than another if its frequency is higher. That is, the most
		// frequent node requires less weight.
		constexpr friend bool operator<(const Node& x, const Node& y) noexcept;

		constexpr size_t get_freq() const noexcept {
			return freq_;
		}

	private:
		const unsigned char ch_;
		// Frequency of character. Must be greater than 0.
		const size_t freq_;
		const Node* const left_;
		const Node* const right_;
};

class Tree {
	public:
		constexpr explicit Tree(const Node* const root) noexcept;
		constexpr explicit Tree(const core::FreqCounter& freq_counter) noexcept;

	private:
		const Node* root_;
};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Count frequencies of characters.
	core::FreqCounter freq_counter;
	input >> freq_counter;

	// Build a tree using counted frequencies.
	Tree tree{freq_counter};

	output << "This is Huffman decompressor." << std::endl;
}

void HuffmanDecompressor::decompress(
		std::istream& input, std::ostream& output) {
	(void)input;
	output << "This is Huffman decompressor." << std::endl;
}

constexpr Node::Node(const unsigned char ch, const size_t freq)
	: Node(ch, freq, nullptr, nullptr) {}

constexpr Node::Node(
		const unsigned char ch,
		const size_t freq,
		const Node* const left,
		const Node* const right)
		: ch_{ch}, freq_{freq}, left_{left}, right_{right} {
	// Zero frequency does not make sense.
	assert(freq > 0);
}

constexpr bool operator<(const Node& x, const Node& y) noexcept {
	return x.get_freq() > y.get_freq();
}

constexpr Tree::Tree(const Node* const root) noexcept: root_{root} {}

constexpr Tree::Tree(
		const core::FreqCounter& freq_counter) noexcept: Tree{nullptr} {
	// Character nodes to store frequencies greater than 0.
	std::vector<const Node*> nodes;

	// Create character nodes with corresponding frequencies greater than 0.
	unsigned char ch{0};
	do {
		if (freq_counter[ch] > 0) {
			nodes.push_back(new Node(ch, freq_counter[ch]));
		}
	} while (++ch != 0);

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes in non-descending order.
		auto compare = [](const Node* const x, const Node* const y) {
			return *x < *y;
		};
		std::sort(nodes.begin(), nodes.end(), compare);

		// Build a tree node.
		const Node* const right{nodes.back()};
		nodes.pop_back();
		const Node* const left{nodes.back()};
		nodes.pop_back();
		const size_t freq{left->get_freq() + right->get_freq()};
		nodes.push_back(new Node(ch, freq, left, right));
	}

	// Replace current `nullptr` root with built root.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

}  // namespace algorithms
