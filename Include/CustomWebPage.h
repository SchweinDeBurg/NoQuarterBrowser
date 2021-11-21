#if !defined(_CustomWebPage_h)
#define _CustomWebPage_h

#include <QDebug>
#include <QWebEnginePage>
#include <QWebEngineProfile>

#include <BrowserWindow.h>

class CustomWebPage: public QWebEnginePage
{
public:
	CustomWebPage(QObject* parent = nullptr);
	CustomWebPage(QWebEngineProfile* profile, QObject* parent = nullptr);
	virtual ~CustomWebPage(void);

protected:
	virtual QWebEnginePage* createWindow(WebWindowType type);

private:
	BrowserWindow* parentBrowserWindow(void);
};

#endif   // _CustomWebPage_h
