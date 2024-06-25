#pragma once

#include <memory>
#include <string>

#include "compressor.h"

namespace core {

class CompressorFactory {
	public:
		virtual ~CompressorFactory() = default;
		virtual std::unique_ptr<Compressor> create(const std::string& name) = 0;
};

} // namespace core