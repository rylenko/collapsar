#include "algorithm/huffman.h"

#include <cassert>
#include <cstddef>

#include <algorithm>
#include <fstream>
#include <ios>
#include <istream>
#include <iterator>
#include <map>
#include <ostream>
#include <stdexcept>
#include <vector>

#include "core/comparator.h"
#include "core/compressor.h"
#include "core/freq_counter.h"

namespace algorithm {

// The direction of the path, which can be represented as an integer. Left
// means 0 and right means 1.
enum PathDirection {
	Left = 0,
	Right = 1,
};

// Path of the tree.
using Path = std::vector<PathDirection>;

// Tree path of each character.
using Paths = std::map<char, Path>;

// Huffman compression tree node to store characters with corresponsing
// frequency or left and right nodes.
class Node {
	public:
		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr Node(char ch, size_t freq) noexcept;

		// Creates a grouping node with left and right branches. In the case of this
		// constructor both cannot be `nullptr`, you need to use another constructor.
		constexpr Node(Node* left, Node* right) noexcept;

		// Destructs the node.
		constexpr ~Node() noexcept;

		// A node is less than another if its frequency is higher. That is, the most
		// frequent node requires less weight.
		friend constexpr bool operator<(const Node& x, const Node& y) noexcept;

		// Recursively calculates character node paths using buffer and its position.
		constexpr void calculate_paths(
			Paths& paths, Path& buffer, size_t buffer_i) const noexcept;

		// Determines that node is another nodes group.
		constexpr bool is_group() const noexcept;

	private:
		// A character with a certain frequency or a random character if the left and
		// right branches are set.
		char ch_;
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
		explicit Tree(const core::FreqCounter& freq_counter) noexcept;

		// Destructs the built tree.
		constexpr ~Tree() noexcept;

		// Calculate paths. So this function calculates tree path for each character.
		Paths calculate_paths() const noexcept;

	private:
		Node* root_;
};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Huffman compression requires double read. So to seek later we need to
	// require input to be ifstream.
	validate_input_is_ifstream_(input);

	// Count frequencies of characters to build a tree with optimal paths.
	core::FreqCounter freq_counter;
	input >> freq_counter;

	// Write input length to the output.
	output << freq_counter.get_total();
	if (output.bad()) {
		throw core::CompressorError("failed to write input's length to output");
	}

	// Build the tree using character frequencies.
	Tree tree{freq_counter};

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

constexpr Node::Node(const char ch, const size_t freq) noexcept
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

constexpr Node::~Node() noexcept {
	// Destruct and deallocate branches if current node is grouping node.
	if (is_group()) {
		left_->~Node();
		delete left_;

		left_->~Node();
		delete left_;
	}
}

constexpr bool operator<(const Node& x, const Node& y) noexcept {
	return x.freq_ > y.freq_;
}

constexpr void Node::calculate_paths(
		Paths& paths, Path& buffer, size_t buffer_i) const noexcept {
	if (is_group()) {
		// Continue recursion through left branch from current buffer position.
		buffer[buffer_i] = PathDirection::Left;
		left_->calculate_paths(paths, buffer, buffer_i + 1);

		// Continue recursion through left branch from current buffer position.
		buffer[buffer_i] = PathDirection::Right;
		right_->calculate_paths(paths, buffer, buffer_i + 1);
	} else {
		// Copy current buffer to the paths map.
		auto inserter = std::back_inserter(paths[ch_]);
		std::ranges::copy_n(buffer.begin(), buffer_i + 1, inserter);
	}
}

constexpr bool Node::is_group() const noexcept {
	// There is no need to check right node against `nullptr` because a grouping
	// node has either 0 branches or both.
	return nullptr != left_;
}

Tree::Tree(const core::FreqCounter& freq_counter) noexcept: root_{nullptr} {
	// Character nodes to store frequencies greater than 0.
	std::vector<Node*> nodes;
	// Create character nodes with corresponding frequencies greater than 0.
	for (auto it = freq_counter.begin(); freq_counter.end() != it; ++it) {
		nodes.push_back(new Node(it->first, it->second));
	}

	// Build a tree of nodes. The most frequent nodes have the shortest path.
	while (nodes.size() > 1) {
		// Sort nodes by frequency descending.
		std::sort(nodes.begin(), nodes.end(), core::LessPtr<Node>{});

		// Group the last two elements into a grouping node.
		Node* const right{nodes.back()};
		nodes.pop_back();
		Node* const left{nodes.back()};
		nodes.pop_back();
		nodes.push_back(new Node{left, right});
	}
	// Replace current `nullptr` root with built root if exists.
	if (!nodes.empty()) {
		root_ = nodes[0];
	}
}

constexpr Tree::~Tree() noexcept {
	if (nullptr != root_) {
		// Call node's destructor to free possible left and right subnodes.
		root_->~Node();
		// Free allocated node.
		delete root_;
	}
}

Paths Tree::calculate_paths() const noexcept {
	Paths paths;
	Path buffer;
	// Calculate paths recursively using buffer from the root node.
	root_->calculate_paths(paths, buffer, 0);
	return paths;
}

}  // namespace algorithm
