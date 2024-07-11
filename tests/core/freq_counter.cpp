#include <sstream>

#include <gtest/gtest.h>

#include "core/freq_counter.h"

TEST(FreqCounter, CountingAndIterator) {
	core::FreqCounter counter;

	// Write test string to the counter.
	std::string str("apple bubble 123  ");
	std::stringstream stream(str);
	stream >> counter;

	// Test total frequencies.
	EXPECT_EQ(counter.get_total(), str.size());

	// Test iterator step by step.
	auto it = counter.begin();

	EXPECT_EQ(it->first, ' ');
	EXPECT_EQ(it->second, 4);
	++it;

	EXPECT_EQ(it->first, '1');
	EXPECT_EQ(it->second, 1);
	++it;

	EXPECT_EQ(it->first, '2');
	EXPECT_EQ(it->second, 1);
	++it;

	EXPECT_EQ(it->first, '3');
	EXPECT_EQ(it->second, 1);
	++it;

	EXPECT_EQ(it->first, 'a');
	EXPECT_EQ(it->second, 1);
	++it;

	EXPECT_EQ(it->first, 'b');
	EXPECT_EQ(it->second, 3);
	++it;

	EXPECT_EQ(it->first, 'e');
	EXPECT_EQ(it->second, 2);
	++it;

	EXPECT_EQ(it->first, 'l');
	EXPECT_EQ(it->second, 2);
	++it;

	EXPECT_EQ(it->first, 'p');
	EXPECT_EQ(it->second, 2);
	++it;

	EXPECT_EQ(it->first, 'u');
	EXPECT_EQ(it->second, 1);
	++it;

	EXPECT_EQ(it, counter.end());
}
