#include "compressor_factory.h"

#include <format>
#include <memory>
#include <string_view>

#include "core/compressor.h"
#include "core/compressor_factory.h"
#include "huffman.h"

namespace algorithm {

std::unique_ptr<core::Compressor> CompressorFactory::create(
		const std::string_view name) const {
	if (name == "huffman") {
		return std::make_unique<HuffmanCompressor>();
	}

	throw core::CompressorFactoryError(
		std::format("unknown compressor \"{}\"", name));
}

}  // namespace algorithm