#pragma once

#include <istream>
#include <ostream>
#include <stdexcept>

namespace core {

// Virtual data decompressor.
class Decompressor {
	public:
		virtual ~Decompressor() = default;

		// Virtual method for decompressing data from `input` to `output`.
		virtual void decompress(std::istream& input, std::ostream& output) = 0;
};

// Decompressor error.
class DecompressorError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core
