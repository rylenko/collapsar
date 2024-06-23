// TODO: docs

#include <iostream>
#include "algorithms/huffman_compressor.h"
#include "algorithms/huffman_decompressor.h"

int main() {
	algorithms::HuffmanCompressor huffman_compressor;
	algorithms::Compressor* compressor = &huffman_compressor;

	algorithms::HuffmanDecompressor huffman_decompressor;
	algorithms::Decompressor* decompressor = &huffman_decompressor;

	compressor->compress(std::cin, std::cout);
	decompressor->decompress(std::cin, std::cout);
	return EXIT_SUCCESS;
}
