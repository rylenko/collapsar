#include "main_window.h"

#include <Qt>
#include <QCoreApplication>
#include <QFileDialog>
#include <QMainWindow>
#include <QPoint>
#include <QPushButton>
#include <QRect>
#include <QSize>
#include <QStringList>

namespace gui {

MainWindow::MainWindow(QWidget* parent): QMainWindow{parent} {
	// TODO: split into the small functions.

	// Create exit button.
	exit_button_ = new QPushButton{"Exit", this};
	// Configure geometry of the exit button.
	exit_button_->setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));
	// Connect file dialog button to the handler.
	connect(
		exit_button_,
		&QPushButton::clicked,
		this,
		&QCoreApplication::quit,
		Qt::QueuedConnection
	);

	// Create file dialog button.
	file_dialog_button_ = new QPushButton{"Select files", this};
	// Configure geometry of the file dialog button.
	file_dialog_button_->setGeometry(QRect(QPoint(100, 200), QSize(200, 50)));
	// Connect file dialog button to the handler.
	connect(
		file_dialog_button_,
		&QPushButton::clicked,
		this,
		&MainWindow::handle_file_dialog_button_
	);

	// Create file names browser.
	file_names_browser_ = new QTextBrowser{this};
	// Configure geometry of the file dialog button.
	file_names_browser_->setGeometry(QRect(QPoint(100, 300), QSize(500, 300)));
	// Set hint text for file names browser.
	file_names_browser_->setText("The file names will be here...");
}

void MainWindow::handle_file_dialog_button_() {
	// Open file dialog to select file names.
	QStringList file_names =
		QFileDialog::getOpenFileNames(this, "Select one or more files");

	// Update file names browser.
	file_names_browser_->setText("");
	for (const auto& file_name : file_names) {
		file_names_browser_->append(file_name);
	}
}

} // namespace gui
