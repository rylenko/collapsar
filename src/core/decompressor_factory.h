#pragma once

#include <cstddef>

#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "decompressor.h"

namespace core {

// Virtual decompressor factory.
class DecompressorFactory {
	public:
		virtual ~DecompressorFactory() = default;

		// Virtual method to get the array of names of available decompressors.
		virtual constexpr std::vector<std::string_view>
			get_names() const noexcept = 0;

		// Virtual method to create a decompressor using passed `name`, if one
		// exists.
		virtual std::unique_ptr<Decompressor> create(
			std::string_view name) const = 0;
};

// Decompressor factory error.
class DecompressorFactoryError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core
