#include "freq_counter.h"

#include <istream>

namespace core {

std::istream& operator>>(std::istream& stream, FreqCounter& counter) noexcept {
	for (unsigned char ch; stream.get(reinterpret_cast<char&>(ch));) {
		counter.count_(ch);
	}
	return stream;
}

void FreqCounter::count_(const unsigned char ch) noexcept {
	counts_[ch]++;
	total_++;
}

}  // namespace core
