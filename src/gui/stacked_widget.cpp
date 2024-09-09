#include "stacked_widget.h"

#include <QWidget>

namespace gui {

void
StackedWidget::add_widgets(
	QWidget* const main_widget,
	QWidget* const receive_files_widget,
	QWidget* const send_files_widget)
{
	addWidget(main_widget);
	main_widget_ = main_widget;

	addWidget(receive_files_widget);
	receive_files_widget_ = receive_files_widget;

	addWidget(send_files_widget);
	send_files_widget_ = send_files_widget;

}

void
StackedWidget::show_main_widget()
{
	setCurrentWidget(main_widget_);
}

void
StackedWidget::show_receive_files_widget()
{
	setCurrentWidget(receive_files_widget_);
}

void
StackedWidget::show_send_files_widget()
{
	setCurrentWidget(send_files_widget_);
}

}
