#include "decompressor_factory.h"

#include <format>
#include <memory>
#include <string_view>

#include "core/decompressor.h"
#include "core/decompressor_factory.h"
#include "huffman.h"

namespace algorithm {

std::unique_ptr<core::Decompressor> DecompressorFactory::create(
		const std::string_view name) const {
	if (name == "huffman") {
		return std::make_unique<HuffmanDecompressor>();
	}

	throw core::DecompressorFactoryError(
		std::format("unknown decompressor \"{}\"", name));
}

}  // namespace algorithm
