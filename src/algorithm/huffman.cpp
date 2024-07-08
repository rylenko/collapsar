#include "algorithm/huffman.h"

#include <cassert>
#include <climits>
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

#include "core/bit.h"
#include "core/comparator.h"
#include "core/compressor.h"
#include "core/freq_counter.h"

namespace algorithm {

inline constexpr size_t BUF_SIZE{4096};

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
			Paths& paths, Path& buf, size_t buf_index) const noexcept;

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

		// Dumps the tree to the passed buffer starting from passed bit index.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

	private:
		Node* root_;
};

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	// Huffman compression requires double read. So to seek later we need to
	// require input to be ifstream.
	validate_input_is_ifstream_(input);

	// Create the buffer with zeros to write to output.
	char output_buf[BUF_SIZE]{};
	size_t output_buf_bit_index{0};

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
	// Write readed input size to the buffer.
	const size_t input_size = freq_counter.get_total();
	std::copy_n(
		reinterpret_cast<const char*>(&input_size), sizeof(input_size), output_buf);
	output_buf_bit_index += sizeof(input_size) * CHAR_BIT;

	// Build the tree using character frequencies.
	Tree tree{freq_counter};
	// Dump the tree to the buffer.
	tree.dump(output_buf, output_buf_bit_index);

	// Get each character tree paths.
	Paths paths = tree.calculate_paths();

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
		Paths& paths, Path& buf, size_t buf_index) const noexcept {
	if (is_group()) {
		// Continue recursion through left branch from current buffer position.
		buf[buf_index] = PathDirection::Left;
		left_->calculate_paths(paths, buf, buf_index + 1);

		// Continue recursion through left branch from current buffer position.
		buf[buf_index] = PathDirection::Right;
		right_->calculate_paths(paths, buf, buf_index + 1);
	} else {
		// Copy current buffer to the paths map.
		auto inserter = std::back_inserter(paths[ch_]);
		std::ranges::copy_n(buf.begin(), buf_index + 1, inserter);
	}
}

// Dumps current node to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void Node::dump(char* const buf, size_t& bit_index) const noexcept {
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
	Path buf;
	// Calculate paths recursively using buffer from the root node.
	root_->calculate_paths(paths, buf, 0);
	return paths;
}

// Dumps the tree to the passed buffer starting from passed bit index.
//
// Make sure that buffer is big enough to store node's dump.
constexpr void Tree::dump(char* const buf, size_t& bit_index) const noexcept {
	root_->dump(buf, bit_index);
}

}  // namespace algorithm
