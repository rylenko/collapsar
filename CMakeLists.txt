cmake_minimum_required(VERSION 3.16)

# Include utilities.
include(FetchContent)

# Configure the project.
project(collapsar VERSION 0.1.0 LANGUAGES CXX)

# Add include directories.
include_directories(src)

# Configure compilation.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug) # TODO: Use via flags

# Add compilation warnings.
if (MSVC)
	add_compile_options(/W4 /WX)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find the Qt 6 library.
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# Setup Qt 6 project.
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
	src/algorithm/compressor_factory.cpp
	src/algorithm/decompressor_factory.cpp
	src/algorithm/huffman.cpp
	src/core/freq_counter.cpp
	src/gui/font.cpp
	src/gui/main_widget.cpp
	src/gui/main_window.cpp
	src/gui/port_validator.cpp
	src/gui/receive_files_widget.cpp
	src/gui/send_files_widget.cpp
	src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
set_target_properties(${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
	# Fetch testing library.
	FetchContent_Declare(googletest
		URL https://github.com/google/googletest/archive/58d77fa8070e8cec2dc1ed015d66b454c8d78850.zip
	)
	FetchContent_MakeAvailable(googletest)
	# Add test files and link them with the test library.
	add_executable(test
		src/algorithm/compressor_factory.cpp
		src/algorithm/decompressor_factory.cpp
		src/algorithm/huffman.cpp
		src/core/freq_counter.cpp

		tests/core/bit.cpp
		tests/core/comparator.cpp
		tests/core/freq_counter.cpp
		tests/algorithm/compressor_factory.cpp
		tests/algorithm/decompressor_factory.cpp
		tests/algorithm/huffman.cpp
	)
	target_link_libraries(test gtest_main)

	# Find sanitizer libraries.
	find_package(ubsan)
	find_package(asan)
	# Add sanitizers to debug builds.
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined -fsanitize=address")
	target_link_options(${PROJECT_NAME}
		BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
	target_link_options(test
		BEFORE PUBLIC -fsanitize=undefined PUBLIC -fsanitize=address)
endif()

