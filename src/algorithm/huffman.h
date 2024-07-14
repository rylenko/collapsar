// TODO: Keep here only compressor and decompressor. Move compress_ to Tree class

#pragma once

#include <cstddef>

#include <istream>
#include <ostream>
#include <map>
#include <vector>

#include "core/compressor.h"
#include "core/decompressor.h"
#include "core/freq_counter.h"

namespace algorithm {

// The direction of the path, which can be represented as an integer. Left
// means 0 and right means 1.
enum class HuffmanTreeDirection {
	Left,
	Right,
};

// Path of the tree.
using HuffmanTreePath = std::vector<HuffmanTreeDirection>;

// Tree path of each character.
using HuffmanTreePaths = std::map<char, HuffmanTreePath>;

// Huffman compression tree node to store characters with corresponsing
// frequency or left and right nodes.
class HuffmanTreeNode {
	public:
		// Creates a character node with the corresponding frequency. The left and
		// right branches are `nullptr`.
		constexpr HuffmanTreeNode(char ch, size_t freq) noexcept;

		// Creates a grouping node with left and right branches. In the case of this
		// constructor both cannot be `nullptr`, you need to use another constructor.
		constexpr HuffmanTreeNode(
			HuffmanTreeNode* left, HuffmanTreeNode* right) noexcept;

		// Destructs the node.
		constexpr ~HuffmanTreeNode() noexcept;

		// A node is less than another if its frequency is higher. That is, the most
		// frequent node requires less weight.
		friend constexpr bool operator<(
			const HuffmanTreeNode& x, const HuffmanTreeNode& y) noexcept;

		// Recursively calculates character node paths using buffer and its position.
		constexpr void calculate_paths(
			HuffmanTreePaths& paths,
			HuffmanTreePath& buf,
			size_t buf_index) const noexcept;

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
		HuffmanTreeNode* left_;
		HuffmanTreeNode* right_;
};

// Huffman compression characters tree.
class HuffmanTree {
	public:
		// Counts the frequencies of content from the `input`, then builds a tree,
		// where the more frequent characters are closer to the root.
		//
		// After successfully construction, the stream must be in the EOF state:
		// eofbit and failbit.
		explicit HuffmanTree(const core::FreqCounter& freq_counter) noexcept;

		// Destructs the built tree.
		constexpr ~HuffmanTree() noexcept;

		// Calculate paths. So this function calculates tree path for each character.
		HuffmanTreePaths calculate_paths() const noexcept;

		// Dumps the tree to the passed buffer starting from passed bit index.
		constexpr void dump(char* buf, size_t& bit_index) const noexcept;

	private:
		HuffmanTreeNode* root_;
};

class HuffmanCompressor: public core::Compressor {
	public:
		void compress(std::istream& input, std::ostream& output) override;

	private:
		// Compresses input's content to output using passed tree.
		static void compress_(
			std::istream& input, const HuffmanTree& tree, std::ostream& output);
};

class HuffmanDecompressor: public core::Decompressor {
	public:
		void decompress(std::istream& input, std::ostream& output) override;
};

}  // namespace algorithm
