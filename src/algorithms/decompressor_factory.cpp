#include "decompressor_factory.h"

#include <memory>
#include <string>

#include "../core/decompressor.h"
#include "huffman.h"

namespace algorithms {

std::unique_ptr<core::Decompressor> DecompressorFactory::create(
	const std::string& name
) {
	if (name == "huffman") {
		return std::make_unique<HuffmanDecompressor>();
	}
	// TODO: exceptions
	return std::make_unique<HuffmanDecompressor>();
}

} // namespace algorithms