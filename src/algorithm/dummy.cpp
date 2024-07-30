#include "algorithm/dummy.h"

#include <algorithm>
#include <istream>
#include <iterator>
#include <ostream>

namespace algorithm {

void redirect_stream(std::istream& input, std::ostream& output) {
	// Redirect input stream to the output stream.
	std::copy(std::istreambuf_iterator<char>(input),
		std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(output));

	// Handle input stream errors.

	// Handle output stream errors.
}

void DummyCompressor::compress(std::istream& input, std::ostream& output) {
	redirect_stream(input, output);
}

void DummyDecompressor::decompress(std::istream& input, std::ostream& output) {
	redirect_stream(input, output);
}

}  // namespace algorithm
