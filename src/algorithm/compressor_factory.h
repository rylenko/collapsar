#pragma once

#include <memory>
#include <string_view>

#include "core/compressor.h"
#include "core/compressor_factory.h"

namespace algorithm {

class CompressorFactory: public core::CompressorFactory<1> {
	public:
		constexpr static Names NAMES{"huffman"};

		constexpr const Names& get_names() const noexcept override {
			return NAMES;
		}

		std::unique_ptr<core::Compressor> create(
			const std::string_view name) const override;
};

}  // namespace algorithm
