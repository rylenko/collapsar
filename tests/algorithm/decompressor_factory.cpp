#include <memory>

#include <gtest/gtest.h>

#include "algorithm/decompressor_factory.h"
#include "algorithm/huffman.h"
#include "core/decompressor.h"
#include "core/decompressor_factory.h"

namespace {

TEST(DecompressorFactory, create) {
	// Create the factory.
	std::unique_ptr<core::DecompressorFactory<1>> factory =
		std::make_unique<algorithm::DecompressorFactory>();

	// Test Huffman's decompressor creation.
	std::unique_ptr<core::Decompressor> decompressor = factory->create("huffman");
	EXPECT_TRUE(
		dynamic_cast<algorithm::HuffmanDecompressor*>(decompressor.get()));

	// Test unknown decompressor creation.
	EXPECT_THROW(
		factory->create("unknown"), algorithm::DecompressorCreationError);
}

}