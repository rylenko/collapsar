#include "algorithm/compressor_factory.h"

#include <format>
#include <memory>
#include <string_view>

#include "algorithm/dummy.h"
#include "algorithm/huffman.h"
#include "core/compressor.h"
#include "core/compressor_factory.h"

namespace algorithm {

std::unique_ptr<core::Compressor>
CompressorFactory::create(const std::string_view name) const
{
	if ("no compression" == name) {
		return std::make_unique<DummyCompressor>();
	} else if ("huffman" == name) {
		return std::make_unique<HuffmanCompressor>();
	}

	throw core::CompressorFactoryError{
		std::format("unknown compressor \"{}\"", name)};
}

}  // namespace algorithm
