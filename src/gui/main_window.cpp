#include "gui/main_window.h"

#include <QMainWindow>

#include "gui/main_widget.h"

namespace gui {

MainWindow::MainWindow(QWidget* parent): QMainWindow{parent} {
	setCentralWidget(new gui::MainWidget{this});
	setWindowTitle(tr("Collapsar"));
}

}  // namespace gui
