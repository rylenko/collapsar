#pragma once

#include <istream>
#include <ostream>

namespace core {

// Virtual data compressor.
class Compressor {
	public:
		virtual ~Compressor() = default;

		// Virtual method for compressing data from `input` to `output`.
		virtual void compress(std::istream& input, std::ostream& output) = 0;
};

}  // namespace core
