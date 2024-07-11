#include <memory>

#include <gtest/gtest.h>

#include "algorithm/compressor_factory.h"
#include "algorithm/huffman.h"
#include "core/compressor.h"
#include "core/compressor_factory.h"

TEST(CompressorFactory, Creation) {
	// Create the factory.
	std::unique_ptr<core::CompressorFactory<1>> factory =
		std::make_unique<algorithm::CompressorFactory>();

	// Test Huffman's compressor creation.
	std::unique_ptr<core::Compressor> compressor = factory->create("huffman");
	EXPECT_TRUE(dynamic_cast<algorithm::HuffmanCompressor*>(compressor.get()));

	// Test unknown compressor creation.
	EXPECT_THROW(factory->create("unknown"), core::CompressorFactoryError);
}
