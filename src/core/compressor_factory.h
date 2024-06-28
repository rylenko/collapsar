#pragma once

#include <cstddef>

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include "compressor.h"

namespace core {

template <std::size_t N>
class CompressorFactory {
	public:
		using Names = std::array<std::string, N>;

		virtual ~CompressorFactory() = default;

		virtual constexpr const Names& get_names() const noexcept = 0;

		virtual std::unique_ptr<Compressor> create(std::string_view name) const = 0;
};

}  // namespace core
