#include <cstddef>

#include <sstream>

#include <gtest/gtest.h>

#include "algorithm/huffman.h"
#include "core/bit.h"

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

	// Test tree dump.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 2), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 2 + 1), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 2 + 2), 0b1);

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 2 + 3), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 2 + 4), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 2 + 5), ',');
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 3 + 5), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 3 + 6), 'H');

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 4 + 6), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 4 + 7), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 4 + 8), ' ');
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 5 + 8), 0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 5 + 9), '!');

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 6 + 9), 0b1);

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 6 + 10), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 6 + 11), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 6 + 12), 'r');
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 7 + 12), 0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 7 + 13), 'w');

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 8 + 13), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 8 + 14), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 8 + 15), 'd');
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 9 + 15), 0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 9 + 16), 'e');

	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 10 + 16), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 10 + 17), 0b0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 10 + 18), 'l');
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 11 + 18), 0);
	EXPECT_EQ(core::bit_read_char(output_chars, CHAR_BIT * 11 + 19), 'o');

	// TODO: Test compressed input data.
}
