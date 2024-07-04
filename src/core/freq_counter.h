#pragma once

#include <cstddef>
#include <istream>
#include <map>
#include <stdexcept>

namespace core {

// Counts the number of characters in total and individually.
//
// Use indexing by `char` to get the count of a specific character. Use
// `get_total()` to get the total count of characters.
class FreqCounter {
	public:
		// Counts characters of the passed stream until EOF.
		//
		// After successfully counting, the stream must be in the EOF state: eofbit
		// and failbit.
		friend std::istream& operator>>(std::istream &input, FreqCounter& counter);

		// Gets a count of the passed `ch`aracter.
		size_t operator[](const char ch) noexcept {
			return counts_[ch];
		}

		// Returns total count of characters.
		constexpr size_t get_total() const noexcept {
			return total_;
		}

	private:
		// Counts new passed `ch`aracter.
		void count_(char ch) noexcept;

		std::map<char, size_t> counts_;
		size_t total_ = 0;
};

// Frequency counter error.
class FreqCounterError: public std::runtime_error {
	using std::runtime_error::runtime_error;
};

}  // namespace core
