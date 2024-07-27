#pragma once

#include <QMainWindow>
#include <QStackedWidget>

namespace gui {

class MainWindow: public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);

	private:
		// Create stacked widget with all widgets of the application.
		QStackedWidget* create_stacked_widget();

		// Sets dark theme to the window's palette.
		void set_dark_theme_();
};

}  // namespace gui
