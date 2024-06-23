#pragma once

#include <istream>
#include <ostream>
#include "compressor.h"

namespace algorithms {

class HuffmanCompressor: public Compressor {
	public:
		void compress(std::istream& input, std::ostream& output) override;
};

} // namespace algorithms
