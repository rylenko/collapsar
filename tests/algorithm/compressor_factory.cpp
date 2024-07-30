#include <memory>

#include <gtest/gtest.h>

#include "algorithm/compressor_factory.h"
#include "algorithm/dummy.h"
#include "algorithm/huffman.h"
#include "core/compressor.h"
#include "core/compressor_factory.h"

TEST(CompressorFactory, Creation) {
	// Create the factory.
	std::unique_ptr<core::CompressorFactory> factory =
		std::make_unique<algorithm::CompressorFactory>();

	// Test dummy compressor creation.
	std::unique_ptr<core::Compressor> compressor =
		factory->create("no compression");
	EXPECT_TRUE(dynamic_cast<algorithm::DummyCompressor*>(compressor.get()));

	// Test Huffman's compressor creation.
	compressor = factory->create("huffman");
	EXPECT_TRUE(dynamic_cast<algorithm::HuffmanCompressor*>(compressor.get()));

	// Test unknown compressor creation.
	EXPECT_THROW(factory->create("unknown"), core::CompressorFactoryError);
}
