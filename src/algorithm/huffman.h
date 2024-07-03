#pragma once

#include <istream>
#include <ostream>

#include "core/compressor.h"
#include "core/decompressor.h"

namespace algorithm {

class HuffmanCompressor: public core::Compressor {
	public:
		void compress(std::istream& input, std::ostream& output) override;

	private:
		// Validates that `input` stream is `std::ifstream`. Otherwise throws an
		// exception.
		static void validate_input_is_ifstream_(const std::istream& input);
};

class HuffmanDecompressor: public core::Decompressor {
	public:
		void decompress(std::istream& input, std::ostream& output) override;
};

}  // namespace algorithm
