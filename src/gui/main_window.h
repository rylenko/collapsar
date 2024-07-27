#pragma once

#include <QMainWindow>
#include <QStackedWidget>

#include "core/compressor_factory.h"

namespace gui {

class MainWindow: public QMainWindow {
	public:
		MainWindow(
			core::CompressorFactory* compressor_factory, QWidget* parent = nullptr);

	private:
		// Create stacked widget with all widgets of the application.
		QStackedWidget* create_stacked_widget(
			core::CompressorFactory* compressor_factory);

		// Sets dark theme to the window's palette.
		void set_dark_theme_();
};

}  // namespace gui
