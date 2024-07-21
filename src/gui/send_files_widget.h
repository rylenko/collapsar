#pragma once

#include <QLayout>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QWidget>

namespace gui {

class SendFilesWidget: public QWidget {
	Q_OBJECT

	public:
		SendFilesWidget(QStackedWidget* stacked_widget, QWidget* parent = nullptr);

	private slots:
		void handle_back_button_();
		void handle_file_dialog_button_();

	private:
		constexpr static unsigned int FONT_SIZE_{13};

		void create_back_button_(QLayout* layout);
		void create_file_dialog_button_(QLayout* layout);
		void create_file_names_browser_(QLayout* layout);

		QStackedWidget* const stacked_widget_;
		QTextBrowser* file_names_browser_;
};

}  // namespace gui
