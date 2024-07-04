#include "core/freq_counter.h"

#include <istream>

namespace core {

std::istream& operator>>(std::istream& stream, FreqCounter& counter) {
	// Count characters from the passed stream.
	for (unsigned char ch; stream.get(reinterpret_cast<char&>(ch));) {
		counter.count_(ch);
	}

	// Validate that EOF reached.
	if (!stream.eof()) {
		throw FreqCounterError("frequency counter did not reach EOF");
	}
	return stream;
}

void FreqCounter::count_(const unsigned char ch) noexcept {
	counts_[ch]++;
	total_++;
}

}  // namespace core
