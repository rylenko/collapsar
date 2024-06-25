#pragma once

#include <memory>
#include <string_view>

#include "compressor.h"

namespace core {

class CompressorFactory {
	public:
		virtual ~CompressorFactory() = default;
		virtual std::unique_ptr<Compressor> create(std::string_view name) = 0;
};

} // namespace core