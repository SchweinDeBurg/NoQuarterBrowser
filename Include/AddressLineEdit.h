#if !defined(_AddressLineEdit_h)
#define _AddressLineEdit_h

#include <QDebug>
#include <QLineEdit>

class AddressLineEdit: public QLineEdit
{
public:
	AddressLineEdit(QWidget* parent = nullptr);
	virtual ~AddressLineEdit(void);

protected:
	virtual void focusInEvent(QFocusEvent* event);
};

#endif   // _AddressLineEdit_h
