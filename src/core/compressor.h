#pragma once

#include <istream>
#include <ostream>
#include <stdexcept>

namespace core {

// Compressor error.
class CompressorError;

// Virtual data compressor.
class Compressor {
	public:
		virtual ~Compressor() = default;

		// Virtual method for compressing data from `input` to `output`.
		virtual void compress(std::istream& input, std::ostream& output) = 0;
};

class CompressorError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core
