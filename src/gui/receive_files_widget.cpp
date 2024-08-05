#include "gui/receive_files_widget.h"

#include <QFont>
#include <QFormLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

#include "gui/font.h"
#include "gui/port_validator.h"
#include "gui/stacked_widget.h"

namespace gui {

ReceiveFilesWidget::ReceiveFilesWidget(
		StackedWidget* const stacked_widget, QWidget* parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create layout and set it to the widget.
	QFormLayout* const layout = new QFormLayout{this};

	// Create widgets.
	create_port_input_(layout);
	create_listen_button_(layout);
	create_back_button_(layout);
}

void ReceiveFilesWidget::handle_back_button_() {
	stacked_widget_->show_main_widget();
}

void ReceiveFilesWidget::handle_listen_button_() {
	// TODO: Make port field inactive.
}

void ReceiveFilesWidget::create_back_button_(QLayout* const layout) {
	// Create button, set font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Back"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&ReceiveFilesWidget::handle_back_button_);
}

void ReceiveFilesWidget::create_listen_button_(QLayout* const layout) {
	// Create button, set font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Listen"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&ReceiveFilesWidget::handle_listen_button_);
}

void ReceiveFilesWidget::create_port_input_(QFormLayout* const layout) {
	// Create input label and set font to it.
	QLabel* const label = new QLabel{tr("Listen port:"), this};
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
