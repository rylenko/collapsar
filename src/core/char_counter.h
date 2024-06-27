#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <limits>

namespace core {

// Counts the number of characters in total and individually.
//
// Use indexing by `char` to get the count of a specific character. Use
// `get_total()` to get the total count of characters.
class CharCounter {
	public:
		// Counts storage type. Indexes are unsigned characters, values are counts.
		using Counts = std::array<
			size_t,
			std::numeric_limits<unsigned char>::max() + 1>;

		// Counts characters from the passed stream.
		friend std::istream& operator>>(std::istream &input, CharCounter& counter);

		// Gets a constant reference to the count of the passed `ch`aracter.
		constexpr Counts::const_reference operator[](char ch) const {
			return counts_[static_cast<unsigned char>(ch)];
		}

		// Returns total count of characters.
		constexpr size_t get_total() const noexcept {
			return total_;
		}

	private:
		// Counts new passed `ch`aracter.
		void count_(const char ch) noexcept;

		Counts counts_{};
		size_t total_ = 0;
};

}  // namespace core
