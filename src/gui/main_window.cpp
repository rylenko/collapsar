#include "gui/main_window.h"

#include <QColor>
#include <QMainWindow>
#include <QPalette>

#include "gui/main_widget.h"

namespace gui {

MainWindow::MainWindow(QWidget* parent): QMainWindow{parent} {
	setCentralWidget(new gui::MainWidget{this});
	setWindowTitle(tr("Collapsar"));
	set_dark_theme_();
}

void MainWindow::set_dark_theme_() {
	// Create palette.
	QPalette palette;
	palette.setColor(QPalette::Window, QColor(53, 53, 53));
	palette.setColor(QPalette::WindowText, Qt::white);
	palette.setColor(QPalette::Base, QColor(25, 25, 25));
	palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
	palette.setColor(QPalette::ToolTipBase, Qt::white);
	palette.setColor(QPalette::ToolTipText, Qt::white);
	palette.setColor(QPalette::Text, Qt::white);
	palette.setColor(QPalette::Button, QColor(53, 53, 53));
	palette.setColor(QPalette::ButtonText, Qt::white);
	palette.setColor(QPalette::BrightText, Qt::red);
	palette.setColor(QPalette::Link, QColor(42, 130, 218));
	palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
	palette.setColor(QPalette::HighlightedText, Qt::black);

	// Set palette.
	setPalette(palette);
}

}  // namespace gui
