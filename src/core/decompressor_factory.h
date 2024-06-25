#pragma once

#include <memory>
#include <string>

#include "compressor.h"

namespace core {

class DecompressorFactory {
	public:
		virtual ~DecompressorFactory() = default;
		virtual std::unique_ptr<Decompressor> create(const std::string& name) = 0;
};

} // namespace core