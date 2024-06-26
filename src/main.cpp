// TODO: docs

#include <iostream>
#include <memory>

#include <QApplication>

#include "algorithms/compressor_factory.h"
#include "algorithms/decompressor_factory.h"
#include "core/analysis.h"
#include "core/compressor.h"
#include "core/compressor_factory.h"
#include "core/decompressor.h"
#include "core/decompressor_factory.h"
#include "gui/main_window.h"

int main(int argc, char** argv) {
	auto compressor_factory = new algorithms::CompressorFactory{};
	auto decompressor_factory = new algorithms::DecompressorFactory{};

	std::unique_ptr<core::Compressor> compressor =
		compressor_factory->create("huffman");
	std::unique_ptr<core::Decompressor> decompressor =
		decompressor_factory->create("huffman");

	compressor->compress(std::cin, std::cout);
	decompressor->decompress(std::cin, std::cout);

	try {
		compressor = compressor_factory->create("invalid-compressor");
	} catch (const algorithms::CompressorCreationError& error) {
		std::cout << "Failed to create compressor: " << error.what() << "."
			<< std::endl;
	}
	try {
		decompressor = decompressor_factory->create("invalid-decompressor");
	} catch (const algorithms::DecompressorCreationError& error) {
		std::cout << "Failed to create decompressor: " << error.what() << "."
			<< std::endl;
	}

	std::cout << std::endl << "Compressors:" << std::endl;
	for (const std::string& name : compressor_factory->get_names()) {
		std::cout << name << std::endl;
	}

	std::cout << std::endl << "Decompressors:" << std::endl;
	for (const std::string& name : decompressor_factory->get_names()) {
		std::cout << name << std::endl;
	}

	delete compressor_factory;
	delete decompressor_factory;

	core::FrequenciesArray frequencies;
	std::cout << "Length: " << core::calculate_frequencies(std::cin, frequencies)
		<< std::endl;
	std::cout << frequencies['a'] << ',' << frequencies['b'] << std::endl;

	QApplication app{argc, argv};
	gui::MainWindow main_window;
	main_window.showMaximized();
	return app.exec();
}
