#pragma once

#include <istream>
#include <ostream>

namespace core {

class Decompressor {
	public:
		virtual ~Decompressor() = default;
		virtual void decompress(std::istream& input, std::ostream& output) = 0;
};

} // namespace core
