#include "gui/receive_files_widget.h"

#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QLayout>
#include <QWidget>

namespace gui {

ReceiveFilesWidget::ReceiveFilesWidget(QWidget* parent): QWidget{parent} {
	// Create grid layout and set it to the widget.
	QLayout* const layout = new QGridLayout{this};
	setLayout(layout);

	// Create label and change its font size.
	QLabel* const label = new QLabel{this};
	label->setText(tr("Receiving files widget."));
	QFont font = label->font();
	font.setPointSize(FONT_SIZE_);
	label->setFont(font);
	layout->addWidget(label);
}

}  // namespace gui
