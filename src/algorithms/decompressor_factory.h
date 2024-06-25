#pragma once

#include <memory>
#include <string_view>

#include "../core/decompressor.h"
#include "../core/decompressor_factory.h"

namespace algorithms {

class DecompressorFactory: public core::DecompressorFactory {
	public:
		std::unique_ptr<core::Decompressor> create(std::string_view name) override;
};

} // namespace algorithms