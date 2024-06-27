#include "frequency_counter.h"

#include <algorithm>
#include <istream>

namespace core {

std::istream& operator>>(std::istream& stream, FrequencyCounter& counter) {
	for (
		unsigned char uchar;
		// Read stream characters to unsigned char variable.
		stream.get(reinterpret_cast<char&>(uchar));
		// Increment total counter after each character.
		counter.total_++
	) {
		// Increment current character counter.
		counter.frequencies_[uchar]++;
	}
	return stream;
}

void FrequencyCounter::reset() noexcept {
	// Reset total count.
	total_ = 0;
	// Fill frequencies array with zeros.
	std::fill(frequencies_.begin(), frequencies_.end(), 0);
}

}  // namespace core
