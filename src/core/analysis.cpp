#include "analysis.h"

#include <algorithm>
#include <array>
#include <cstddef>
#include <istream>
#include <limits>

namespace core {

// TODO: test
size_t calculate_frequencies(std::istream& stream, FrequenciesArray& to) {
	// Ensure that we can store all unsigned char frequencies.
	static_assert(std::size(to) >= std::numeric_limits<unsigned char>::max() + 1);

	// Fill output array with zeros.
	std::fill(to.begin(), to.end(), 0);

	size_t len{0};
	unsigned char ch;

	// Read characters from the stream to register frequencies.
	for (; stream.get(reinterpret_cast<char&>(ch)); len++) {
		to[ch]++;
	}
	return len;
}

} // namespace core
