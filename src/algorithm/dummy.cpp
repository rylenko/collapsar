#include "algorithm/dummy.h"

#include <cstddef>

#include <istream>
#include <ostream>

#include "core/compressor.h"

namespace algorithm {

constexpr size_t REDIRECT_BUF_SIZE{8192};

void redirect_stream(std::istream& input, std::ostream& output) {
	// Buffer to read from the input stream.
	char buf[REDIRECT_BUF_SIZE];

	// Read characters from the input stream and write them to the output stream.
	while (!input.eof()) {
		input.read(buf, sizeof(buf));
		if (input.bad() || (input.fail() && !input.eof())) {
			throw core::CompressorError{"failed to read from the input stream"};
		}

		if (output.write(buf, input.gcount()).bad()) {
			throw core::CompressorError{"failed to write to the output stream"};
		}
	}
}

void DummyCompressor::compress(std::istream& input, std::ostream& output) {
	redirect_stream(input, output);
}

void DummyDecompressor::decompress(std::istream& input, std::ostream& output) {
	redirect_stream(input, output);
}

}  // namespace algorithm
