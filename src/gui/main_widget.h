#pragma once

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
		QTextBrowser* file_names_browser_;
};

}  // namespace gui
