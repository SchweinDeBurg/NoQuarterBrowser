#include <QTimer>

#include <AddressLineEdit.h>

AddressLineEdit::AddressLineEdit(QWidget* parent):
QLineEdit(parent)
{
}

AddressLineEdit::~AddressLineEdit(void)
{
}

void AddressLineEdit::focusInEvent(QFocusEvent* event)
{
	// https://stackoverflow.com/a/51807268
	QLineEdit::focusInEvent(event);
	QTimer::singleShot(0, this, &QLineEdit::selectAll);
}
