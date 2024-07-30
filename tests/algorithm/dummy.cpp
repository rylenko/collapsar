#include <climits>
#include <cstddef>

#include <ios>
#include <sstream>

#include <gtest/gtest.h>

#include "algorithm/dummy.h"

TEST(Dummy, CompressAndDecompressEmpty) {
	// Create streams.
	std::stringstream decompressed;
	std::stringstream compressed;

	// Create compressor and compress.
	algorithm::DummyCompressor compressor;
	compressor.compress(decompressed, compressed);
	// Test stream states.
	EXPECT_TRUE(decompressed.eof() && decompressed.fail() && !decompressed.bad());
	EXPECT_TRUE(compressed.good());
	// Test compressed content.
	EXPECT_EQ(compressed.view(), "");

	// Clean decompressed stream.
	std::stringstream{}.swap(decompressed);
	EXPECT_EQ(decompressed.view(), "");

	// Create decompressor and decompress.
	algorithm::DummyDecompressor decompressor;
	decompressor.decompress(compressed, decompressed);
	// Test stream states.
	EXPECT_TRUE(compressed.eof() && compressed.fail() && !compressed.bad());
	EXPECT_TRUE(decompressed.good());
	// Test that decompressed stream is empty.
	EXPECT_EQ(decompressed.view(), "");
}

TEST(Dummy, CompressAndDecompressSmall) {
	// Create streams.
	std::stringstream decompressed{"Hello, world!"};
	std::stringstream compressed;

	// Create compressor and compress.
	algorithm::DummyCompressor compressor;
	compressor.compress(decompressed, compressed);
	// Test stream states.
	EXPECT_TRUE(decompressed.eof() && decompressed.fail() && !decompressed.bad());
	EXPECT_TRUE(compressed.good());
	// Test compressed content.
	EXPECT_EQ(compressed.view(), "Hello, world!");

	// Zeroize decompressed stream.
	std::stringstream{}.swap(decompressed);
	EXPECT_EQ(decompressed.view(), "");

	// Create decompressor and decompress.
	algorithm::DummyDecompressor decompressor;
	decompressor.decompress(compressed, decompressed);
	// Test stream states.
	EXPECT_TRUE(compressed.eof() && compressed.fail() && !compressed.bad());
	EXPECT_TRUE(decompressed.good());
	// Test decompressed content.
	EXPECT_EQ(decompressed.view(), "Hello, world!");
}

// TODO: Add tests with huge decompressed stream
