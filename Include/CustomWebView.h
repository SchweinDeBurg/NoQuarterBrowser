#if !defined(_CustomWebView_h)
#define _CustomWebView_h

#include <QDebug>
#include <QWebEngineView>

class CustomWebView: public QWebEngineView
{
public:
	CustomWebView(QWidget* parent = nullptr);
	virtual ~CustomWebView(void);

protected:
	virtual void contextMenuEvent(QContextMenuEvent* event);
};

#endif   // _CustomWebView_h
