#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "core/compressor.h"
#include "core/compressor_factory.h"

namespace algorithm {

class CompressorFactory: public core::CompressorFactory {
	public:
		constexpr std::vector<std::string_view> get_names() const noexcept override {
			return {"huffman"};
		}

		std::unique_ptr<core::Compressor> create(
			const std::string_view name) const override;
};

}  // namespace algorithm
