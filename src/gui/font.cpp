#include "gui/font.h"

#include <QFont>
#include <QWidget>

namespace gui {

constexpr unsigned int FONT_SIZE{13};

void set_font(QWidget* const widget) {
	QFont font = widget->font();
	font.setPointSize(FONT_SIZE);
	widget->setFont(font);
}

}
