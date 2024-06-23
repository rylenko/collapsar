#include <istream>
#include <ostream>
#include "huffman_decompressor.h"

namespace algorithms {

void HuffmanDecompressor::decompress(std::istream& input, std::ostream& output)
{
	output << "This is Huffman decompressor." << std::endl;
}

} // namespace algorithms
