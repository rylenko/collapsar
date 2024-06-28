cmake_minimum_required(VERSION 3.16)

# Configure the project.
project(collapsar VERSION 0.1.0 LANGUAGES CXX)

# Configure compilation.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
if (MSVC)
	# Add compile warnings.
	add_compile_options(/W4 /WX)
else()
	# Add compile warnings.
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Configure Qt.
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()
qt_add_executable(${PROJECT_NAME}
	src/algorithms/compressor_factory.cpp
	src/algorithms/decompressor_factory.cpp
	src/algorithms/huffman.cpp
	src/core/char_counter.cpp
	src/gui/main_window.cpp
	src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)
set_target_properties(${PROJECT_NAME} PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON)
