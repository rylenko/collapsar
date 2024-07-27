#include "gui/main_widget.h"

#include <QCoreApplication>
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QResizeEvent>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "gui/font.h"
#include "gui/receive_files_widget.h"
#include "gui/send_files_widget.h"

namespace gui {

MainWidget::MainWidget(
		QStackedWidget* const stacked_widget, QWidget* const parent)
		: QWidget{parent}, stacked_widget_{stacked_widget} {
	// Create main layout.
	QVBoxLayout* const layout = new QVBoxLayout{this};

	// Add title to main layout.
	create_title_(layout);

	// Create buttons layout and add buttons to it.
	QLayout* const buttons_layout = new QHBoxLayout;
	create_send_files_button_(buttons_layout);
	create_receive_files_button_(buttons_layout);
	create_quit_button_(buttons_layout);

	// Add buttons layout to main layout.
	layout->addLayout(buttons_layout);
}

void MainWidget::resizeEvent(QResizeEvent* event) {
	// Call parent method.
	QWidget::resizeEvent(event);

	// Adjust contents margins.
	adjust_margins_();
}

void MainWidget::adjust_margins_() {
	// Calculate width and height margins.
	int width_margins = width() * 0.25;
	int height_margins = height() * 0.45;

	// Set margins to the main layout.
	layout()->setContentsMargins(
		width_margins, height_margins, width_margins, height_margins);
}

void MainWidget::create_title_(QLayout* const layout) {
	// Create title, align it, set text with font and add title to the layout.
	QLabel* const title = new QLabel{this};
	title->setAlignment(Qt::AlignCenter);
	title->setText(tr("🎇 Collapsar"));
	set_font(title, FontSize::Title);
	layout->addWidget(title);
}

void MainWidget::create_send_files_button_(QLayout* const layout) {
	// Create button, add font and add button to the layout.
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

void MainWidget::create_receive_files_button_(QLayout* const layout) {
	// Create button, add font and add button to the layout.
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

void MainWidget::create_quit_button_(QLayout* const layout) {
	// Create button, add font and add button to the layout.
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
