#pragma once

#include <istream>
#include <ostream>

#include "core/compressor.h"
#include "core/decompressor.h"

namespace algorithm {

class DummyCompressor: public core::Compressor {
	public:
		void compress(std::istream& input, std::ostream& output) override;
};

class DummyDecompressor: public core::Decompressor {
	public:
		void decompress(std::istream& input, std::ostream& output) override;
};

}  // namespace algorithm
