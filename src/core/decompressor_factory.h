#pragma once

#include <memory>
#include <string_view>

#include "compressor.h"

namespace core {

class DecompressorFactory {
	public:
		virtual ~DecompressorFactory() = default;
		virtual std::unique_ptr<Decompressor> create(std::string_view name) = 0;
};

} // namespace core