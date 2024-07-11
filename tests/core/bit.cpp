#include <climits>

#include <gtest/gtest.h>

#include "core/bit.h"

TEST(Bit, Set) {
	char buf[3]{0, 0, 0};

	core::bit_set(buf, CHAR_BIT * 3 - 1);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 0);
	EXPECT_EQ(buf[2], 1);

	core::bit_set(buf, CHAR_BIT * 2 - 3);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 4);
	EXPECT_EQ(buf[2], 1);

	core::bit_set(buf, CHAR_BIT * 2 - 6);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 36);
	EXPECT_EQ(buf[2], 1);
}

TEST(Bit, Clear) {
	char buf[3]{0, 36, 1};

	core::bit_clear(buf, CHAR_BIT * 2 - 6);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 4);
	EXPECT_EQ(buf[2], 1);

	core::bit_clear(buf, CHAR_BIT * 2 - 3);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 0);
	EXPECT_EQ(buf[2], 1);

	core::bit_clear(buf, CHAR_BIT * 3 - 1);
	EXPECT_EQ(buf[0], 0);
	EXPECT_EQ(buf[1], 0);
	EXPECT_EQ(buf[2], 0);
}

TEST(Bit, Write) {
	char buf[3]{0, 0, 0};

	core::bit_write(buf, 2, CHAR_MIN);
	EXPECT_EQ(buf[0], CHAR_MIN >> 2);
	EXPECT_EQ(buf[1], (CHAR_MIN & 0b11) << (CHAR_BIT - 2));
	EXPECT_EQ(buf[2], 0);

	core::bit_write(buf, CHAR_BIT + 1, CHAR_MAX);
	EXPECT_EQ(buf[0], CHAR_MIN >> 2);
	EXPECT_EQ(buf[1], (CHAR_MIN & 0b10) << (CHAR_BIT - 2) | CHAR_MAX >> 1);
	// Static cast need here to make testing library happy.
	EXPECT_EQ(buf[2], static_cast<char>((CHAR_MAX & 0b1) << (CHAR_BIT - 1)));
}
