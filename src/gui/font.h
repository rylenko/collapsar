#pragma once

#include <QWidget>

namespace gui {

enum class FontSize {
	Regular = 13,
	Title = 25,
};

void set_font(QWidget* widget, FontSize size = FontSize::Regular);

}
