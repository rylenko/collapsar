#include <sstream>

#include <gtest/gtest.h>

#include "core/freq_counter.h"

namespace {

TEST(FreqCounter, all) {
	core::FreqCounter counter;

	// Write test string to the counter.
	std::string str("apple bubble 123  ");
	std::stringstream stream(str);
	stream >> counter;

	// Test total frequencies.
	EXPECT_EQ(counter.get_total(), str.size());
	// Test indexing operator.
	EXPECT_EQ(counter[' '], 4);
	EXPECT_EQ(counter['1'], 1);
	EXPECT_EQ(counter['2'], 1);
	EXPECT_EQ(counter['3'], 1);
	EXPECT_EQ(counter['a'], 1);
	EXPECT_EQ(counter['b'], 3);
	EXPECT_EQ(counter['e'], 2);
	EXPECT_EQ(counter['l'], 2);
	EXPECT_EQ(counter['p'], 2);
	EXPECT_EQ(counter['u'], 1);
}

}