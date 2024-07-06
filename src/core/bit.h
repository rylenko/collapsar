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

}  // namespace core
