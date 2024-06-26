#pragma once

#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include "../core/compressor.h"
#include "../core/compressor_factory.h"

namespace algorithms {

inline constexpr std::array<std::string, 1> COMPRESSOR_NAMES{"huffman"};

class CompressorCreationError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class CompressorFactory:
	public core::CompressorFactory<std::size(COMPRESSOR_NAMES)>
{
	public:
		inline constexpr const std::array<std::string, std::size(COMPRESSOR_NAMES)>&
		get_names() const noexcept override {
			return COMPRESSOR_NAMES;
		}

		std::unique_ptr<core::Compressor>
		create(const std::string_view name) const override;
};

} // namespace algorithms