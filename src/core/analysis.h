#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <limits>

namespace core {

using FrequenciesArray =
	std::array<size_t, std::numeric_limits<unsigned char>::max() + 1>;

size_t calculate_frequencies(std::istream& stream, FrequenciesArray& to);

} // namespace core
