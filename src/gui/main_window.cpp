#include "gui/main_window.h"

#include <QColor>
#include <QMainWindow>
#include <QPalette>
#include <QStackedWidget>

#include "gui/main_widget.h"
#include "gui/receive_files_widget.h"
#include "gui/send_files_widget.h"

namespace gui {

MainWindow::MainWindow(QWidget* parent): QMainWindow{parent} {
	// Create stacked widget and make it central.
	setCentralWidget(create_stacked_widget());
	// Set title.
	setWindowTitle(tr("Collapsar"));
	// Set theme.
	set_dark_theme_();
}

QStackedWidget* MainWindow::create_stacked_widget() {
	QStackedWidget* const stacked_widget = new QStackedWidget{this};
	stacked_widget->addWidget(new MainWidget{stacked_widget, this});
	stacked_widget->addWidget(new SendFilesWidget{stacked_widget, this});
	stacked_widget->addWidget(new ReceiveFilesWidget{this});

	return stacked_widget;
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
