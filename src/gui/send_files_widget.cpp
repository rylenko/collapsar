#include "gui/send_files_widget.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFont>
#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStackedWidget>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>

#include "core/compressor_factory.h"
#include "gui/font.h"
#include "gui/port_validator.h"

namespace gui {

SendFilesWidget::SendFilesWidget(
		QStackedWidget* const stacked_widget,
		core::CompressorFactory* const compressor_factory,
		QWidget* parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create grid layout and set it to the widget.
	QLayout* const layout = new QVBoxLayout{this};

	// Create widgets.
	create_host_input_(layout);
	create_port_input_(layout);
	create_algorithm_choice_(layout, compressor_factory);
	create_file_names_browser_(layout);
	create_file_dialog_button_(layout);
	create_back_button_(layout);
}

void SendFilesWidget::handle_back_button_() {
	stacked_widget_->setCurrentIndex(0);
}

void SendFilesWidget::handle_file_dialog_button_() {
	// Open file dialog to select file names.
	QStringList file_names =
		QFileDialog::getOpenFileNames(this, tr("Select one or more files"));

	// Reset content of the browser if some files selected.
	if (!file_names.empty()) {
		file_names_browser_->setText("");
	}
	// Append selected filenames to the browser.
	for (const auto& file_name : file_names) {
		file_names_browser_->append(file_name);
	}
}

void SendFilesWidget::create_algorithm_choice_(
		QLayout* const layout, core::CompressorFactory* const compressor_factory) {
	// Create and add combo box title, set font to it and add to the layout.
	QLabel* const label = new QLabel{this};
	label->setText(tr("Select the algorithm:"));
	set_font(label);
	layout->addWidget(label);

	// Create and fill combo box, set font to it and add to the layout.
	QComboBox* const box = new QComboBox{this};
	for (auto name : compressor_factory->get_names()) {
		box->addItem(name.data());
	}
	set_font(box);
	layout->addWidget(box);
}

void SendFilesWidget::create_back_button_(QLayout* const layout) {
	// Create button, set font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Back"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button, &QPushButton::clicked, this, &SendFilesWidget::handle_back_button_);
}

void SendFilesWidget::create_file_dialog_button_(QLayout* const layout) {
	// Create button, set font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Select files"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&SendFilesWidget::handle_file_dialog_button_);
}

void SendFilesWidget::create_file_names_browser_(QLayout* const layout) {
	// Create browser, set font to it and add it to the layout.
	file_names_browser_ = new QTextBrowser{this};
	file_names_browser_->setText(tr("The file names will be here..."));
	set_font(file_names_browser_);
	layout->addWidget(file_names_browser_);
}

void SendFilesWidget::create_host_input_(QLayout* const layout) {
	// Create input, set placeholder, set font and add input to the layout.
	QLineEdit* const input = new QLineEdit{this};
	input->setPlaceholderText(tr("Input receiver's host..."));
	set_font(input);
	layout->addWidget(input);
}

void SendFilesWidget::create_port_input_(QLayout* const layout) {
	// Create input, set placeholder, set validator, set font and add input to the
	// layout.
	QLineEdit* const input = new QLineEdit{this};
	input->setPlaceholderText(tr("Input receiver's port..."));
	input->setValidator(new PortValidator{this});
	set_font(input);
	layout->addWidget(input);
}

}  // namespace gui
