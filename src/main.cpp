// TODO: Make docs and comments more readable.

#include <QApplication>

#include "algorithm/compressor_factory.h"
#include "gui/main_window.h"

int
main(int argc, char** argv)
{
	// Create compressor factory.
	algorithm::CompressorFactory compressor_factory;

	// Create Qt application.
	QApplication app{argc, argv};
	// Show maximized main window.
	gui::MainWindow main_window{&compressor_factory};
	main_window.showMaximized();
	// Execute Qt application.
	return app.exec();
}
