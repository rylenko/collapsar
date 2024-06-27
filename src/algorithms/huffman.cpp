#include "huffman.h"

#include <istream>
#include <ostream>

namespace algorithms {

void HuffmanCompressor::compress(std::istream& input, std::ostream& output) {
	output << "This is Huffman compressor." << std::endl;
}

void HuffmanDecompressor::decompress(
		std::istream& input,
		std::ostream& output) {
	output << "This is Huffman decompressor." << std::endl;
}

}  // namespace algorithms
