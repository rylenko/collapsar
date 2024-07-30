#include <memory>

#include <gtest/gtest.h>

#include "algorithm/decompressor_factory.h"
#include "algorithm/dummy.h"
#include "algorithm/huffman.h"
#include "core/decompressor.h"
#include "core/decompressor_factory.h"

TEST(DecompressorFactory, Creation) {
	// Create the factory.
	std::unique_ptr<core::DecompressorFactory> factory =
		std::make_unique<algorithm::DecompressorFactory>();

	// Test dummy decompressor creation.
	std::unique_ptr<core::Decompressor> decompressor =
		factory->create("no decompression");
	EXPECT_TRUE(dynamic_cast<algorithm::DummyDecompressor*>(decompressor.get()));

	// Test Huffman's decompressor creation.
	decompressor = factory->create("huffman");
	EXPECT_TRUE(
		dynamic_cast<algorithm::HuffmanDecompressor*>(decompressor.get()));

	// Test unknown decompressor creation.
	EXPECT_THROW(factory->create("unknown"), core::DecompressorFactoryError);
}
