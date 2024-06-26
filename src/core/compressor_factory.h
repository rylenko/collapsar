#pragma once

#include <array>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

#include "compressor.h"

namespace core {

template <std::size_t N>
class CompressorFactory {
	public:
		virtual ~CompressorFactory() = default;

		virtual constexpr const std::array<std::string, N>&
		get_names() const noexcept = 0;

		virtual std::unique_ptr<Compressor> create(std::string_view name) = 0;
};

} // namespace core