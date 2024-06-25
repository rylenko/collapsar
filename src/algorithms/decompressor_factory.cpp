#include "decompressor_factory.h"

#include <memory>
#include <string_view>

#include "../core/decompressor.h"
#include "huffman.h"

namespace algorithms {

std::unique_ptr<core::Decompressor> DecompressorFactory::create(
	std::string_view name
) {
	if (name == "huffman") {
		return std::make_unique<HuffmanDecompressor>();
	}
	// TODO: exceptions
	return std::make_unique<HuffmanDecompressor>();
}

} // namespace algorithms