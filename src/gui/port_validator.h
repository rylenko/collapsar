#pragma once

#include <QObject>
#include <QString>
#include <QValidator>

namespace gui {

// Validates that input contains valid port number.
class PortValidator: public QValidator {
	Q_OBJECT

	public:
		using QValidator::QValidator;

		virtual State validate(QString& input, int& pos) const override;
};

}
