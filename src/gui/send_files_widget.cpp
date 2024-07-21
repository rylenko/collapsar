#include "gui/send_files_widget.h"

#include <QCoreApplication>
#include <QFileDialog>
#include <QFont>
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QStringList>
#include <QWidget>

namespace gui {

SendFilesWidget::SendFilesWidget(
		QStackedWidget* const stacked_widget, QWidget* parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create grid layout and set it to the widget.
	QGridLayout* const layout = new QGridLayout{this};
	setLayout(layout);

	// Add other widgets.
	create_file_names_browser_(layout);
	create_file_dialog_button_(layout);
	create_back_button_(layout);
}

void SendFilesWidget::create_back_button_(QLayout* const layout) {
	// Create button.
	QPushButton* const button = new QPushButton{tr("&Back"), this};

	// Change font size.
	QFont font = button->font();
	font.setPointSize(FONT_SIZE_);
	button->setFont(font);

	// Add button to the layout.
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button, &QPushButton::clicked, this, &SendFilesWidget::handle_back_button_);
}

void SendFilesWidget::create_file_dialog_button_(QLayout* const layout) {
	// Create button.
	QPushButton* const button = new QPushButton{tr("&Select files"), this};

	// Change font size.
	QFont font = button->font();
	font.setPointSize(FONT_SIZE_);
	button->setFont(font);

	// Add button to the layout.
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&SendFilesWidget::handle_file_dialog_button_);
}

void SendFilesWidget::create_file_names_browser_(QLayout* const layout) {
	// Create text browser.
	file_names_browser_ = new QTextBrowser{this};

	// Set placeholder.
	file_names_browser_->setText(tr("The file names will be here..."));

	// Change font size.
	QFont font = file_names_browser_->font();
	font.setPointSize(FONT_SIZE_);
	file_names_browser_->setFont(font);

	// Add widget to the layout.
	layout->addWidget(file_names_browser_);
}

void SendFilesWidget::handle_back_button_() {
	stacked_widget_->setCurrentIndex(0);
}

void SendFilesWidget::handle_file_dialog_button_() {
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
