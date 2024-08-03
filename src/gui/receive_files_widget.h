#pragma once

#include <QFormLayout>
#include <QLayout>
#include <QWidget>

#include "gui/stacked_widget.h"

namespace gui {

class ReceiveFilesWidget: public QWidget {
	public:
		ReceiveFilesWidget(StackedWidget* stacked_widget, QWidget* parent = nullptr);

	private slots:
		void handle_back_button_();

	private:
		void create_back_button_(QLayout* layout);
		void create_port_input_(QFormLayout* layout);

		StackedWidget* const stacked_widget_;
};

}  // namespace gui
