#include "decompressor_factory.h"

#include <format>
#include <memory>
#include <string_view>

#include "../core/decompressor.h"
#include "huffman.h"

namespace algorithms {

std::unique_ptr<core::Decompressor> DecompressorFactory::create(
	const std::string_view name
) {
	if (name == "huffman") {
		return std::make_unique<HuffmanDecompressor>();
	}

	throw DecompressorCreationError(
		std::format("unknown decompressor \"{}\"", name)
	);
}

} // namespace algorithms