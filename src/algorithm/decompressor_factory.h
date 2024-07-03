#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>

#include "core/decompressor.h"
#include "core/decompressor_factory.h"

namespace algorithm {

class DecompressorCreationError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class DecompressorFactory: public core::DecompressorFactory<1> {
	public:
		constexpr static Names NAMES{"huffman"};

		constexpr const Names& get_names() const noexcept override {
			return NAMES;
		}

		std::unique_ptr<core::Decompressor> create(
			const std::string_view name) const override;
};

}  // namespace algorithm
