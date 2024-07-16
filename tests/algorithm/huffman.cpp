#include <cstddef>

#include <sstream>

#include <gtest/gtest.h>

#include "algorithm/huffman.h"
#include "core/bit.h"

TEST(Huffman, CompressAndDecompressEmpty) {
	// Create streams.
	std::istringstream input;
	std::ostringstream output;

	// Create compressor and compress.
	algorithm::HuffmanCompressor compressor;
	compressor.compress(input, output);

	// Test stream states.
	EXPECT_TRUE(input.eof() && input.fail() && !input.bad());
	EXPECT_TRUE(output.good());

	// Get output.
	const char* const output_chars{output.view().data()};

	// Test input length.
	EXPECT_EQ(output_chars[0], '0');
	EXPECT_EQ(output.tellp(), 1);

	// TODO: Add decompression test.
}

TEST(Huffman, CompressAndDecompressSmall) {
	// Create streams.
	std::istringstream input{"Hello, world!"};
	std::ostringstream output;

	// Create compressor and compress.
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

	// Test 'H' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 19), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 20), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 21), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 22), 0b1);

	// Test 'e' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 23), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 24), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 25), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 26), 0b1);

	// Test 'll' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 27), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 28), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 29), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 30), 0b0);

	// Test 'o' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 31), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 32), 0b1);

	// Test ',' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 33), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 34), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 35), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 36), 0b0);

	// Test ' ' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 37), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 38), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 39), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 40), 0b0);

	// Test 'w' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 41), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 42), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 43), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 44), 0b1);

	// Test 'o' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 45), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 46), 0b1);

	// Test 'r' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 47), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 48), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 49), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 50), 0b0);

	// Test 'l' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 51), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 52), 0b0);

	// Test 'd' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 53), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 54), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 55), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 56), 0b0);

	// Test 'd' compression.
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 57), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 58), 0b0);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 59), 0b1);
	EXPECT_EQ(core::bit_get(output_chars, CHAR_BIT * 12 + 60), 0b1);

	// TODO: Add decompression test.
}

// TODO: Add tests with huge input stream
