#pragma once

#include <istream>
#include <ostream>

namespace core {

class Compressor {
	public:
		virtual ~Compressor() = default;

		virtual void compress(std::istream& input, std::ostream& output) = 0;
};

}  // namespace core
