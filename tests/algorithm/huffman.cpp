#include <climits>

#include <sstream>
#include <string_view>

#include <gtest/gtest.h>

#include "algorithm/huffman.h"

TEST(Huffman, CompressAndDecompress) {
	// Create streams.
	std::istringstream input{"Hello, world!"};
	std::ostringstream output;

	// Create compressor.
	algorithm::HuffmanCompressor compressor;
	compressor.compress(input, output);

	// Test stream states.
	EXPECT_TRUE(input.eof() && input.fail() && !input.bad());
	EXPECT_TRUE(output.good());

	// Get output.
	const char* const output_chars{output.view().data()};

	// Test input length.
	EXPECT_EQ(output_chars[0], '1');
	EXPECT_EQ(output_chars[1], '3');
}
