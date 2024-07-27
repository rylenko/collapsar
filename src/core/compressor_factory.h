#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "compressor.h"

namespace core {

// Virtual compressor factory.
class CompressorFactory {
	public:
		virtual ~CompressorFactory() = default;

		// Virtual method to get the array of names of available compressors.
		//
		// TODO: Make it constexpr.
		virtual std::vector<std::string_view> get_names() const noexcept = 0;

		// Virtual method to create a compressor using passed `name`, if one exists.
		virtual std::unique_ptr<Compressor> create(std::string_view name) const = 0;
};

// Compressor factory error.
class CompressorFactoryError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core
