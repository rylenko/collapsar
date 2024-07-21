#include "main_widget.h"

#include <Qt>
#include <QCoreApplication>
#include <QFileDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

namespace gui {

MainWidget::MainWidget(QWidget* parent): QWidget{parent} {
	// Create grid layout and set it to the widget.
	QGridLayout* const layout = new QGridLayout{this};
	setLayout(layout);

	// Create file names browser.
	file_names_browser_ = new QTextBrowser{this};
	file_names_browser_->setText(tr("The file names will be here..."));
	layout->addWidget(file_names_browser_, 0, 0, 1, 2);

	// Create file dialog button.
	QPushButton* const file_dialog_button =
		new QPushButton{tr("Select files"), this};
	connect(
		file_dialog_button,
		&QPushButton::clicked,
		this,
		&MainWidget::handle_file_dialog_button_
	);
	layout->addWidget(file_dialog_button, 1, 0);

	// Create exit button.
	QPushButton* const exit_button = new QPushButton{tr("Exit"), this};
	connect(
		exit_button,
		&QPushButton::clicked,
		this,
		&QCoreApplication::quit,
		Qt::QueuedConnection
	);
	layout->addWidget(exit_button, 1, 1);
}

void MainWidget::handle_file_dialog_button_() {
	// Open file dialog to select file names.
	QStringList file_names =
		QFileDialog::getOpenFileNames(this, tr("Select one or more files"));

	// Update file names browser.
	if (!file_names.empty()) {
		file_names_browser_->setText("");
	}
	for (const auto& file_name : file_names) {
		file_names_browser_->append(file_name);
	}
}

}  // namespace gui
