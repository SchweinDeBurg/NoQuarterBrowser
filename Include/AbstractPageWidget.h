#if !defined(_AbstractPageWidget_h)
#define _AbstractPageWidget_h

#include <QDebug>

#include <BrowserTabWidget.h>
#include <BrowserWindow.h>

class AbstractPageWidget: public QWidget
{
protected:
	AbstractPageWidget(BrowserTabWidget* tabWidget, QWidget* parent = nullptr);
	virtual ~AbstractPageWidget(void) = 0;

protected:
	BrowserWindow* parentBrowserWindow(void);

protected:
	BrowserTabWidget* m_tabWidget;
};

inline AbstractPageWidget::AbstractPageWidget(BrowserTabWidget* tabWidget, QWidget* parent):
QWidget(parent),
m_tabWidget(tabWidget)
{
}

#endif   // _AbstractPageWidget_h
