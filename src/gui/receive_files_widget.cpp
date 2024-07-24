#include "gui/receive_files_widget.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "gui/font.h"

namespace gui {

ReceiveFilesWidget::ReceiveFilesWidget(
		QStackedWidget* const stacked_widget, QWidget* parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create grid layout and set it to the widget.
	QLayout* const layout = new QGridLayout{this};
	setLayout(layout);

	// Create label and change its font size.
	QLabel* const label = new QLabel{this};
	label->setText(tr("Receiving files widget."));
	set_font(label);
	layout->addWidget(label);

	// Create widgets.
	create_back_button_(layout);
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

void ReceiveFilesWidget::handle_back_button_() {
	stacked_widget_->setCurrentIndex(0);
}

}  // namespace gui
