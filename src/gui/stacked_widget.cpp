#include "stacked_widget.h"

#include <QStackedWidget>
#include <QWidget>

namespace gui {

enum StackedWidgetIndex {
	MainWidgetIndex = 0,
	ReceiveFilesWidget = 1,
	SendFilesWidget = 2,
}

StackedWidget::StackedWidget(
		QWidget* const main_widget,
		QWidget* const receive_files_widget,
		QWidget* const send_files_widget,
		QWidget* const parent)
		: QStackedWidget(parent) {
	// Insert widgets to their indexes.
	insertWidget(MainWidgetIndex, main_widget);
	insertWidget(ReceiveFilesWidget, receive_files_widget);
	insertWidget(SendFilesWidget, send_files_widget);
}

void StackedWidget::show_main_widget() {
	setCurrentIndex(MainWidgetIndex);
}

void StackedWidget::show_receive_files_widget() {
	setCurrentIndex(ReceiveFilesIndex);
}

void StackedWidget::show_send_files_widget() {
	setCurrentIndex(SendFilesIndex);
}
