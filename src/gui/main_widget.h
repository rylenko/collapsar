#pragma once

#include <QLayout>
#include <QTextBrowser>
#include <QWidget>

namespace gui {

class MainWidget: public QWidget {
	Q_OBJECT

	public:
		explicit MainWidget(QWidget* parent = nullptr);

	private slots:
		void handle_file_dialog_button_();

	private:
		constexpr static unsigned int FONT_SIZE_{13};

		void create_file_dialog_button_(QLayout* layout);
		void create_file_names_browser_(QLayout* layout);
		void create_quit_button_(QLayout* layout);

		QTextBrowser* file_names_browser_;
};

}  // namespace gui
