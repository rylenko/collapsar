#pragma once

#include <QMainWindow>
#include <QStackedWidget>

namespace gui {

class MainWindow: public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);

	private:
		QStackedWidget* create_stacked_widget();
		void set_dark_theme_();
};

}  // namespace gui
