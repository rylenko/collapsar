#pragma once

#include <memory>
#include <string_view>

#include "core/decompressor.h"
#include "core/decompressor_factory.h"

namespace algorithm {

class DecompressorFactory: public core::DecompressorFactory {
	public:
		constexpr std::vector<std::string_view> get_names() const noexcept override {
			return {"no decompression", "huffman"};
		}

		std::unique_ptr<core::Decompressor> create(
			const std::string_view name) const override;
};

}  // namespace algorithm
