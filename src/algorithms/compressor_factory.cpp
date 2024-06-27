#include "compressor_factory.h"

#include <array>
#include <format>
#include <memory>
#include <string_view>

#include "../core/compressor.h"
#include "huffman.h"

namespace algorithms {

std::unique_ptr<core::Compressor> CompressorFactory::create(
		const std::string_view name) const {
	if (name == "huffman") {
		return std::make_unique<HuffmanCompressor>();
	}

	throw CompressorCreationError(std::format("unknown compressor \"{}\"", name));
}

}  // namespace algorithms
