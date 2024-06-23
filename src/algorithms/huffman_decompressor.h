#pragma once

#include <istream>
#include <ostream>
#include "decompressor.h"

namespace algorithms {

class HuffmanDecompressor: public Decompressor {
	public:
		void decompress(std::istream& input, std::ostream& output) override;
};

} // namespace algorithms
