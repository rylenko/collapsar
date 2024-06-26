#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QWidget>

namespace gui {

class MainWindow: public QMainWindow {
	Q_OBJECT

	public:
		explicit MainWindow(QWidget* parent = nullptr);

	private slots:
		void handle_file_dialog_button_();

	private:
		QPushButton* exit_button_;
		QPushButton* file_dialog_button_;
		QTextBrowser* file_names_browser_;

};

} // namespace gui
