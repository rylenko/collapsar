#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "../core/decompressor.h"
#include "../core/decompressor_factory.h"

namespace algorithms {

inline constexpr std::array<std::string, 1> DECOMPRESSOR_NAMES{"huffman"};

class DecompressorCreationError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class DecompressorFactory:
	public core::DecompressorFactory<std::size(DECOMPRESSOR_NAMES)>
{
	public:
		inline constexpr
		const std::array<std::string, std::size(DECOMPRESSOR_NAMES)>&
		get_names() const noexcept override {
			return DECOMPRESSOR_NAMES;
		}

		std::unique_ptr<core::Decompressor>
		create(const std::string_view name) const override;
};

} // namespace algorithms