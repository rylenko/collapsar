#pragma once

#include <QLayout>
#include <QStackedWidget>
#include <QWidget>

namespace gui {

class ReceiveFilesWidget: public QWidget {
	public:
		ReceiveFilesWidget(
			QStackedWidget* stacked_widget, QWidget* parent = nullptr);

	private slots:
		void handle_back_button_();

	private:
		constexpr static unsigned int FONT_SIZE_{13};

		void create_back_button_(QLayout* layout);

		QStackedWidget* const stacked_widget_;
};

}  // namespace gui
