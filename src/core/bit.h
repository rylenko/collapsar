#pragma once

#include <climits>
#include <cstddef>

namespace core {

// Clears the bit in the passed index.
constexpr void bit_clear(char* const buf, const size_t index) noexcept {
	const size_t left_shift{CHAR_BIT - index % CHAR_BIT - 1};
	buf[index / CHAR_BIT] &= ~(1 << left_shift);
}

// Gets bit value from the passed buffer.
constexpr char bit_get(const char* const buf, const size_t index) noexcept {
	const size_t left_shift{CHAR_BIT - index % CHAR_BIT - 1};
	return buf[index / CHAR_BIT] >> left_shift & 0b1;
}

// Sets the bit in the passed index.
constexpr void bit_set(char* const buf, const size_t index) noexcept {
	const size_t left_shift{CHAR_BIT - index % CHAR_BIT - 1};
	buf[index / CHAR_BIT] |= 1 << left_shift;
}

// Writes character to the buffer starting from passed index.
//
// Make sure the buffer contains at least `CHAR_BIT` bits after index.
constexpr void bit_write(
		char* const buf, const size_t index, const char ch) noexcept {
	buf[index / CHAR_BIT] |= ch >> index % CHAR_BIT;
	buf[index / CHAR_BIT + 1] |= ch << (CHAR_BIT - index % CHAR_BIT);
}

}  // namespace core
