#include "gui/main_widget.h"

#include <QCoreApplication>
#include <QFont>
#include <QGridLayout>
#include <QLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QWidget>

#include "gui/font.h"
#include "gui/receive_files_widget.h"
#include "gui/send_files_widget.h"

namespace gui {

MainWidget::MainWidget(
		QStackedWidget* const stacked_widget, QWidget* const parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create grid layout and set it to the widget.
	QLayout* const layout = new QGridLayout{this};
	setLayout(layout);

	// Add widget and quit buttons.
	create_send_files_button_(layout);
	create_receive_files_button_(layout);
	create_quit_button_(layout);
}

void MainWidget::create_receive_files_button_(QLayout* const layout) {
	// Create button, add font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Receive files"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&MainWidget::handle_receive_files_button_);
}

void MainWidget::create_send_files_button_(QLayout* const layout) {
	// Create button, add font to it and add it to the layout.
	QPushButton* const button = new QPushButton{tr("&Send files"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&MainWidget::handle_send_files_button_);
}

void MainWidget::create_quit_button_(QLayout* const layout) {
	// Update button font and add button to the layout.
	QPushButton* const button = new QPushButton{tr("&Quit"), this};
	set_font(button);
	layout->addWidget(button);

	// Connect button to the handler.
	connect(
		button,
		&QPushButton::clicked,
		this,
		&QCoreApplication::quit,
		Qt::QueuedConnection);
}

void MainWidget::handle_send_files_button_() {
	stacked_widget_->setCurrentIndex(1);
}

void MainWidget::handle_receive_files_button_() {
	stacked_widget_->setCurrentIndex(2);
}

}  // namespace gui
