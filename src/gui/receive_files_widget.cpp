#include "gui/receive_files_widget.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

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
	QFont font = label->font();
	font.setPointSize(FONT_SIZE_);
	label->setFont(font);
	layout->addWidget(label);

	// Create widgets.
	create_back_button_(layout);
}

void ReceiveFilesWidget::create_back_button_(QLayout* const layout) {
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
		button,
		&QPushButton::clicked,
		this,
		&ReceiveFilesWidget::handle_back_button_);
}

void ReceiveFilesWidget::handle_back_button_() {
	stacked_widget_->setCurrentIndex(0);
}

}  // namespace gui
