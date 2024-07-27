#pragma once

#include <QFormLayout>
#include <QLayout>
#include <QStackedWidget>
#include <QTextBrowser>
#include <QWidget>

#include "core/compressor_factory.h"

namespace gui {

class SendFilesWidget: public QWidget {
	Q_OBJECT

	public:
		SendFilesWidget(
			QStackedWidget* stacked_widget,
			core::CompressorFactory* compressor_factory,
			QWidget* parent = nullptr);

	private slots:
		void handle_back_button_();
		void handle_file_dialog_button_();

	private:
		void create_algorithm_choice_(
			QFormLayout* layout, core::CompressorFactory* compressor_factory);
		void create_back_button_(QLayout* layout);
		void create_file_dialog_button_(QLayout* layout);
		void create_file_names_browser_(QFormLayout* layout);
		void create_host_input_(QFormLayout* layout);
		void create_port_input_(QFormLayout* layout);

		QStackedWidget* const stacked_widget_;
		QTextBrowser* file_names_browser_;
};

}  // namespace gui
