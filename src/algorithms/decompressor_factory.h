#pragma once

#include <memory>
#include <string>

#include "../core/decompressor.h"
#include "../core/decompressor_factory.h"

namespace algorithms {

class DecompressorFactory: public core::DecompressorFactory {
	public:
		std::unique_ptr<core::Decompressor> create(const std::string& name) override;
};

} // namespace algorithms