#pragma once

#include <cstddef>

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "compressor.h"

namespace core {

// Virtual compressor factory.
template <std::size_t N>
class CompressorFactory {
	public:
		// Array of names of available compressors.
		using Names = std::array<std::string, N>;

		virtual ~CompressorFactory() = default;

		// Virtual method to get the array of names of available compressors.
		virtual constexpr const Names& get_names() const noexcept = 0;

		// Virtual method to create a compressor using passed `name`, if one exists.
		virtual std::unique_ptr<Compressor> create(std::string_view name) const = 0;
};

}  // namespace core
