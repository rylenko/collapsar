#pragma once

#include <istream>
#include <ostream>

namespace core {

// Virtual data decompressor.
class Decompressor {
	public:
		virtual ~Decompressor() = default;

		// Virtual method for decompressing data from `input` to `output`.
		virtual void decompress(std::istream& input, std::ostream& output) = 0;
};

}  // namespace core
