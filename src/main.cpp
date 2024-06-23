#include <iostream>
#include "algorithms/huffman.h"

int main() {
	std::string hw{"Hello, world!"};
	std::cout << hw << std::endl;

	unsigned char* data = reinterpret_cast<unsigned char*>(
		const_cast<char*>(hw.c_str())
	);
	size_t data_len = hw.size();
	huffman_compress(data, data_len);

	std::cout << '[' << hw << ']' << std::endl;
	return EXIT_SUCCESS;
}
