#include <climits>

#include <gtest/gtest.h>

#include "core/bit.h"

TEST(bit, set_single) {
	char ch{0};

	core::bit_set(&ch, CHAR_BIT - 1);
	EXPECT_EQ(ch, 1);

	core::bit_set(&ch, CHAR_BIT - 3);
	EXPECT_EQ(ch, 5);

	core::bit_set(&ch, CHAR_BIT - 4);
	EXPECT_EQ(ch, 13);
}

TEST(bit, set_multiple) {
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

TEST(bit, clear_single) {
	char ch{13};

	core::bit_clear(&ch, CHAR_BIT - 4);
	EXPECT_EQ(ch, 5);

	core::bit_clear(&ch, CHAR_BIT - 3);
	EXPECT_EQ(ch, 1);

	core::bit_clear(&ch, CHAR_BIT - 1);
	EXPECT_EQ(ch, 0);
}

TEST(bit, clear_multiple) {
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
