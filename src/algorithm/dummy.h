#pragma once

#include <istream>
#include <ostream>

#include "core/compressor.h"
#include "core/decompressor.h"

namespace algorithm {

// Dummy compressor, that does not compress data but simply redirects input to
// output.
class DummyCompressor: public core::Compressor {
	public:
		void compress(std::istream& input, std::ostream& output) override;
};

// Dummy decompressor, that does not compress data but simply redirects input
// to output.
class DummyDecompressor: public core::Decompressor {
	public:
		void decompress(std::istream& input, std::ostream& output) override;
};

}  // namespace algorithm
