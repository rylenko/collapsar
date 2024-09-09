#include "gui/port_validator.h"

#include <QString>
#include <QValidator>

namespace gui {

QValidator::State
PortValidator::validate(QString& input, [[maybe_unused]] int& pos) const
{
	// Empty input is invalid port number.
	if (input.isEmpty())
		return QValidator::State::Intermediate;

	// Try to parse an integer.
	bool isInt;
	int value = input.toInt(&isInt);

	// Validate parsed integer.
	if (isInt && (value >= 0) && (value <= 65535)) {
		return QValidator::State::Acceptable;
	}
	return QValidator::State::Invalid;
}

}
