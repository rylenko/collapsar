#pragma once

#include <cstddef>

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "compressor.h"

namespace core {

// Virtual decompressor factory.
template <std::size_t N>
class DecompressorFactory {
	public:
		// Array of names of available decompressors.
		using Names = std::array<std::string, N>;

		virtual ~DecompressorFactory() = default;

		// Virtual method to get the array of names of available decompressors.
		virtual constexpr const Names& get_names() const noexcept = 0;

		// Virtual method to create a decompressor using passed `name`, if one
		// exists.
		virtual std::unique_ptr<Decompressor> create(
			std::string_view name) const = 0;
};

}  // namespace core
