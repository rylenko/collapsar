#include <climits>
#include <cstddef>

#include <gtest/gtest.h>

#include "core/bit.h"

TEST(Bit, Get) {
	const char buf[]{0, 0b100100, 1};

	// Test first byte.
	for (size_t index{0}; index < CHAR_BIT; ++index) {
		EXPECT_EQ(core::bit_get(buf, index), 0b0);
	}

	// Test second byte.
	for (size_t index{CHAR_BIT}; index < CHAR_BIT * 2 - 6; ++index) {
		EXPECT_EQ(core::bit_get(buf, index), 0b0);
	}
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 6), 0b1);
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 5), 0b0);
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 4), 0b0);
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 3), 0b1);
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 2), 0b0);
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 2 - 1), 0b0);

	// Test third byte.
	for (size_t index{CHAR_BIT * 2}; index < CHAR_BIT * 3 - 1; ++index) {
		EXPECT_EQ(core::bit_get(buf, index), 0b0);
	}
	EXPECT_EQ(core::bit_get(buf, CHAR_BIT * 3 - 1), 0b1);
}


TEST(Bit, Clear) {
	char buf[]{0, 36, 1};

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

TEST(Bit, ReadChar) {
	char buf_1[]{'X' >> (CHAR_BIT - 3), static_cast<char>('X' << 3)};
	EXPECT_EQ(core::bit_read_char(buf_1, CHAR_BIT - 3), 'X');

	char buf_2[]{'\0', 'X' >> (CHAR_BIT - 6), static_cast<char>('X' << 6)};
	EXPECT_EQ(core::bit_read_char(buf_2, CHAR_BIT * 2 - 6), 'X');

	char buf_3[]{'X', '\0', '\0'};
	EXPECT_EQ(core::bit_read_char(buf_3, 0), 'X');
}

TEST(Bit, Set) {
	char buf[]{0, 0, 0};

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

TEST(Bit, WriteChar) {
	char buf[]{0, 0, 0};

	core::bit_write_char(buf, 2, CHAR_MIN);
	EXPECT_EQ(buf[0], CHAR_MIN >> 2);
	EXPECT_EQ(buf[1], (CHAR_MIN & 0b11) << (CHAR_BIT - 2));
	EXPECT_EQ(buf[2], 0);

	core::bit_write_char(buf, CHAR_BIT + 1, CHAR_MAX);
	EXPECT_EQ(buf[0], CHAR_MIN >> 2);
	EXPECT_EQ(buf[1], (CHAR_MIN & 0b10) << (CHAR_BIT - 2) | CHAR_MAX >> 1);
	EXPECT_EQ(buf[2], static_cast<char>((CHAR_MAX & 0b1) << (CHAR_BIT - 1)));
}
