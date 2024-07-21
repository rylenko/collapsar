#pragma once

#include <QLayout>
#include <QStackedWidget>
#include <QWidget>

namespace gui {

class MainWidget: public QWidget {
	Q_OBJECT

	public:
		MainWidget(QStackedWidget* stacked_widget, QWidget* parent = nullptr);

	private slots:
		void handle_receive_files_button_();
		void handle_send_files_button_();

	private:
		constexpr static unsigned int FONT_SIZE_{13};

		// Methods to create main widget elements.
		void create_send_files_button_(QLayout* layout);
		void create_receive_files_button_(QLayout* layout);
		void create_quit_button_(QLayout* layout);

		// Stacked widgets.
		QStackedWidget* const stacked_widget_;
};

}  // namespace gui
