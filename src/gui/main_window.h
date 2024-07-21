#pragma once

#include <QMainWindow>

namespace gui {

class MainWindow: public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);

	private:
		void set_dark_theme_();
};

}  // namespace gui
