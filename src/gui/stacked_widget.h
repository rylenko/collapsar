#pragma once

#include <QStackedWidget>
#include <QWidget>

namespace gui {

// Stack of all application widgets.
class StackedWidget: public QStackedWidget {
	public:
		using QStackedWidget::QStackedWidget;

		void add_widgets(
			QWidget* main_widget,
			QWidget* receive_files_widget,
			QWidget* send_files_widget);

		void show_main_widget();
		void show_receive_files_widget();
		void show_send_files_widget();

	private:
		QWidget* main_widget_;
		QWidget* receive_files_widget_;
		QWidget* send_files_widget_;
};

}
