#include "gui/font.h"

#include <QFont>
#include <QWidget>

namespace gui {

void set_font(QWidget* const widget, const FontSize size) {
	QFont font = widget->font();
	font.setPointSize(static_cast<int>(size));
	widget->setFont(font);
}

}
