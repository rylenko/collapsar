#pragma once

namespace core {

// Like `std::less`, but with pointers dereferencing.
template <typename T>
struct LessPtr {
	constexpr bool
	operator()(const T* const lhs, const T* const rhs) const
	{
		return *lhs < *rhs;
	}
};

}  // namespace core
