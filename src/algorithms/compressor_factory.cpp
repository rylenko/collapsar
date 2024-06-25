#include "compressor_factory.h"

#include <memory>
#include <string>

#include "../core/compressor.h"
#include "huffman.h"

namespace algorithms {

std::unique_ptr<core::Compressor> CompressorFactory::create(
	const std::string& name
) {
	if (name == "huffman") {
		return std::make_unique<HuffmanCompressor>();
	}
	// TODO: exceptions
	return std::make_unique<HuffmanCompressor>();
}

} // namespace algorithms