// TODO: docs

#include <iostream>
#include <memory>

#include "algorithms/compressor_factory.h"
#include "algorithms/decompressor_factory.h"
#include "core/compressor.h"
#include "core/compressor_factory.h"
#include "core/decompressor.h"
#include "core/decompressor_factory.h"

int main() {
	core::CompressorFactory* compressor_factory =
		new algorithms::CompressorFactory();
	core::DecompressorFactory* decompressor_factory =
		new algorithms::DecompressorFactory();

	std::unique_ptr<core::Compressor> compressor =
		compressor_factory->create("huffman");
	std::unique_ptr<core::Decompressor> decompressor =
		decompressor_factory->create("huffman");

	compressor->compress(std::cin, std::cout);
	decompressor->decompress(std::cin, std::cout);

	delete compressor_factory;
	delete decompressor_factory;
	return EXIT_SUCCESS;
}
