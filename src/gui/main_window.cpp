// TODO: Doc stacked widget, add enum Page::Main, Page::SendFiles, Page::ReceiveFiles

#include "gui/main_window.h"

#include <QColor>
#include <QMainWindow>
#include <QPalette>
#include <QStackedWidget>

#include "core/compressor_factory.h"
#include "gui/main_widget.h"
#include "gui/receive_files_widget.h"
#include "gui/send_files_widget.h"

namespace gui {

MainWindow::MainWindow(
		core::CompressorFactory* const compressor_factory, QWidget* parent)
		: QMainWindow{parent} {
	// Create stacked widget and make it central.
	setCentralWidget(create_stacked_widget(compressor_factory));
	// Set title.
	setWindowTitle(tr("Collapsar"));
	// Set theme.
	set_dark_theme_();
}

QStackedWidget* MainWindow::create_stacked_widget(
		core::CompressorFactory* const compressor_factory) {
	QStackedWidget* const stacked_widget = new QStackedWidget{this};
	stacked_widget->addWidget(new MainWidget{stacked_widget, this});
	stacked_widget->addWidget(
		new SendFilesWidget{stacked_widget, compressor_factory, this});
	stacked_widget->addWidget(new ReceiveFilesWidget{stacked_widget, this});

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
	palette.setColor(QPalette::PlaceholderText, Qt::gray);
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
