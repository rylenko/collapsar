#include "gui/send_files_widget.h"

#include <QComboBox>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFont>
#include <QFormLayout>
#include <QIntValidator>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QStringList>
#include <QWidget>

#include "core/compressor_factory.h"
#include "gui/font.h"
#include "gui/port_validator.h"
#include "gui/stacked_widget.h"

namespace gui {

SendFilesWidget::SendFilesWidget(
		StackedWidget* const stacked_widget,
		core::CompressorFactory* const compressor_factory,
		QWidget* parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create grid layout and set it to the widget.
	QFormLayout* const layout = new QFormLayout{this};

	// Create widgets.
	create_host_input_(layout);
	create_port_input_(layout);
	create_algorithm_choice_(layout, compressor_factory);
	create_file_names_browser_(layout);
	create_file_dialog_button_(layout);
	create_back_button_(layout);
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

void SendFilesWidget::handle_back_button_() {
	stacked_widget_->show_main_widget();
}

void SendFilesWidget::create_algorithm_choice_(
		QFormLayout* const layout,
		core::CompressorFactory* const compressor_factory) {
	// Create combo box label and set font to it.
	QLabel* const label = new QLabel{tr("Algorithm:"), this};
	set_font(label);

	// Create and fill combo box, set font to it and add to the layout.
	QComboBox* const box = new QComboBox{this};
	for (auto name : compressor_factory->get_names()) {
		box->addItem(name.data());
	}
	set_font(box);

	// Add label and combo box to the layout.
	layout->addRow(label, box);
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

void SendFilesWidget::create_file_names_browser_(QFormLayout* const layout) {
	// Create browser label and set font to it.
	QLabel* const label = new QLabel{tr("Files:"), this};
	set_font(label);

	// Create browser and set font to it.
	file_names_browser_ = new QTextBrowser{this};
	set_font(file_names_browser_);

	// Add label and browser to the layout.
	layout->addRow(label, file_names_browser_);
}

void SendFilesWidget::create_host_input_(QFormLayout* const layout) {
	// Create input label and set font to it.
	QLabel* const label = new QLabel{tr("Host:"), this};
	set_font(label);

	// Create input, set placeholder and font.
	QLineEdit* const input = new QLineEdit{this};
	input->setPlaceholderText(tr("255.255.255.255"));
	set_font(input);

	// Add label and input to the layout.
	layout->addRow(label, input);
}

void SendFilesWidget::create_port_input_(QFormLayout* const layout) {
	// Create input label and set font to it.
	QLabel* const label = new QLabel{tr("Port:"), this};
	set_font(label);

	// Create input, set placeholder, validator and font.
	QLineEdit* const input = new QLineEdit{this};
	input->setPlaceholderText(tr("8888"));
	input->setValidator(new PortValidator{this});
	set_font(input);

	// Add label and input to the layout.
	layout->addRow(label, input);
}

}  // namespace gui
