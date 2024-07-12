#include <algorithm>

#include <gtest/gtest.h>

#include "core/comparator.h"

TEST(Comparator, LessPtr) {
	// Create array of integer pointers.
	int* number_ptrs[]{
		new int{5}, new int{-2}, new int{0}, new int{0}, new int{1}, new int{-5}};
	// Sort the array of integer pointers using comparator.
	std::ranges::sort(number_ptrs, core::LessPtr<int>{});

	// Test comparator.
	EXPECT_EQ(*number_ptrs[0], -5);
	EXPECT_EQ(*number_ptrs[1], -2);
	EXPECT_EQ(*number_ptrs[2], 0);
	EXPECT_EQ(*number_ptrs[3], 0);
	EXPECT_EQ(*number_ptrs[4], 1);
	EXPECT_EQ(*number_ptrs[5], 5);

	// Clean allocated numbers.
	for (auto ptr : number_ptrs) {
		delete ptr;
	}
}
