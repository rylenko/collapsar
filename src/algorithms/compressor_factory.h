#pragma once

#include <memory>
#include <string>

#include "../core/compressor.h"
#include "../core/compressor_factory.h"

namespace algorithms {

class CompressorFactory: public core::CompressorFactory {
	public:
		std::unique_ptr<core::Compressor> create(const std::string& name) override;
};

} // namespace algorithms