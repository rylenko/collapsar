#pragma once

#include <QLayout>
#include <QResizeEvent>
#include <QWidget>

#include "gui/stacked_widget.h"

namespace gui {

class MainWidget: public QWidget {
	Q_OBJECT

	public:
		MainWidget(StackedWidget* stacked_widget, QWidget* parent = nullptr);

	protected:
		void resizeEvent(QResizeEvent* event) override;

	private slots:
		void handle_receive_files_button_();
		void handle_send_files_button_();

	private:
		// Adjusts contents margins of main layout.
		void adjust_margins_();

		// Methods to create main widget elements.
		void create_title_(QLayout* layout);
		void create_send_files_button_(QLayout* layout);
		void create_receive_files_button_(QLayout* layout);
		void create_quit_button_(QLayout* layout);

		StackedWidget* const stacked_widget_;
};

}  // namespace gui
