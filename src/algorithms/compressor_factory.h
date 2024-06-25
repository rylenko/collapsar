#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>

#include "../core/compressor.h"
#include "../core/compressor_factory.h"

namespace algorithms {

class CompressorCreationError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

class CompressorFactory: public core::CompressorFactory {
	public:
		std::unique_ptr<core::Compressor> create(
			const std::string_view name
		) override;
};

} // namespace algorithms