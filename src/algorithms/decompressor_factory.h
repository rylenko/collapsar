#pragma once

#include <memory>
#include <stdexcept>
#include <string_view>

#include "../core/decompressor.h"
#include "../core/decompressor_factory.h"

namespace algorithms {

class DecompressorFactory: public core::DecompressorFactory {
	public:
		std::unique_ptr<core::Decompressor> create(
			const std::string_view name
		) override;
};

class DecompressorCreationError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

} // namespace algorithms