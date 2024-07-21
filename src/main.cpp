#include <QApplication>

#include "gui/main_window.h"

int main(int argc, char** argv) {
	// Create Qt application.
	QApplication app{argc, argv};

	// Show maximized main window.
	gui::MainWindow main_window;
	main_window.showMaximized();

	// Execute Qt application.
	return app.exec();
}
