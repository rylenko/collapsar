#pragma once

#include <array>
#include <cstddef>
#include <istream>
#include <limits>

namespace core {

class FrequencyCounter {
	public:
		using Counts = std::array<
			size_t,
			std::numeric_limits<unsigned char>::max() + 1>;

		friend std::istream& operator>>(
			std::istream &input,
			FrequencyCounter& counter);

		constexpr Counts::const_reference operator[](unsigned char ch) const {
			return frequencies_[ch];
		}

		constexpr size_t get_total() const noexcept {
			return total_;
		}

		void reset() noexcept;

	private:
		size_t total_ = 0;
		Counts frequencies_{};
};

}  // namespace core
