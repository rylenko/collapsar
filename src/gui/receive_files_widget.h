#pragma once

#include <QWidget>

namespace gui {

class ReceiveFilesWidget: public QWidget {
	public:
		explicit ReceiveFilesWidget(QWidget* parent = nullptr);

	private:
		constexpr static unsigned int FONT_SIZE_{13};
};

}  // namespace gui
