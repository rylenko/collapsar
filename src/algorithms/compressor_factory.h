#pragma once

#include <memory>
#include <string_view>

#include "../core/compressor.h"
#include "../core/compressor_factory.h"

namespace algorithms {

class CompressorFactory: public core::CompressorFactory {
	public:
		std::unique_ptr<core::Compressor> create(std::string_view name) override;
};

} // namespace algorithms