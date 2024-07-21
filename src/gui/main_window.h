#pragma once

#include <QMainWindow>

namespace gui {

class MainWindow: public QMainWindow {
	public:
		explicit MainWindow(QWidget* parent = nullptr);
};

}  // namespace gui
