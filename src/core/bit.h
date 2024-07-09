#pragma once

#include <climits>
#include <cstddef>

namespace core {

// Sets the bit in the passed index.
constexpr void bit_set(char* const buffer, const size_t index) noexcept {
	const size_t left_shift{CHAR_BIT - index % CHAR_BIT - 1};
	buffer[index / CHAR_BIT] |= 1 << left_shift;
}

// Clears the bit in the passed index.
constexpr void bit_clear(char* const buffer, const size_t index) noexcept {
	const size_t left_shift{CHAR_BIT - index % CHAR_BIT - 1};
	buffer[index / CHAR_BIT] &= ~(1 << left_shift);
}

// Writes character to the buffer starting from passed index.
//
// Make sure the buffer contains at least `CHAR_BIT` bits after index.
constexpr void bit_write(
		char* const buffer, const size_t index, const char ch) noexcept {
	buffer[index / CHAR_BIT] |= ch >> index % CHAR_BIT;
	buffer[index / CHAR_BIT + 1] |= ch << (CHAR_BIT - index % CHAR_BIT);
}

}  // namespace core
