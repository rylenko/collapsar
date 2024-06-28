#include "char_counter.h"

#include <istream>

namespace core {

std::istream& operator>>(std::istream& stream, CharCounter& counter) noexcept {
	for (char ch; stream.get(ch);) {
		counter.count_(ch);
	}
	return stream;
}

void CharCounter::count_(const char ch) noexcept {
	counts_[static_cast<unsigned char>(ch)]++;
	total_++;
}

}  // namespace core
