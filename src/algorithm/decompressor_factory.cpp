#include "algorithm/decompressor_factory.h"

#include <format>
#include <memory>
#include <string_view>

#include "algorithm/dummy.h"
#include "algorithm/huffman.h"
#include "core/decompressor.h"
#include "core/decompressor_factory.h"

namespace algorithm {

std::unique_ptr<core::Decompressor> DecompressorFactory::create(
		const std::string_view name) const {
	if ("-" == name) {
		return std::make_unique<DummyDecompressor>();
	} else if ("huffman" == name) {
		return std::make_unique<HuffmanDecompressor>();
	}

	throw core::DecompressorFactoryError(
		std::format("unknown decompressor \"{}\"", name));
}

}  // namespace algorithm
