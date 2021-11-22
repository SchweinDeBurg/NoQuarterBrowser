#if !defined(_BrowserPageWidget_h)
#define _BrowserPageWidget_h

#include <QDebug>
#include <QFrame>
#include <QToolBar>
#include <QUrl>
#include <QWebEngineDownloadItem>
#include <QWidget>

#include <AbstractPageWidget.h>
#include <AddressLineEdit.h>
#include <CommonStuff.h>
#include <CustomWebView.h>

class BrowserPageWidget: public AbstractPageWidget
{
public:
	BrowserPageWidget(BrowserTabWidget* tabWidget, QWidget* parent = nullptr);
	virtual ~BrowserPageWidget(void);

public:
	void focusAddressBar(Qt::FocusReason reason);
	void setWebViewUrl(const QUrl& url);
	void zoomWebView(ZoomDirection direction);
	void refreshWebView(void);

public:
	unsigned int webViewZoomPercent(void) const;

private:
	void onAddressBarReturnPressed(void);
	void onWebViewLoadProgress(int progress);
	void onWebViewLoadFinished(bool ok);
	void onWebViewDownloadRequested(QWebEngineDownloadItem* download);
	void onFocusAddressBar(void);

private:
	QAction* m_focusAddressBarAction;

public:
	CustomWebView* webView(void) const;
	AddressLineEdit* addressBar(void) const;

private:
	QFrame* m_viewFrame;
	CustomWebView* m_webView;
	QToolBar* m_buttonsBar;
	AddressLineEdit* m_addressBar;
};

inline void BrowserPageWidget::focusAddressBar(Qt::FocusReason reason)
{
	m_addressBar->setFocus(reason);
}

inline void BrowserPageWidget::setWebViewUrl(const QUrl& url)
{
	m_webView->setUrl(url);
}

inline void BrowserPageWidget::refreshWebView(void)
{
	m_webView->reload();
}

inline CustomWebView* BrowserPageWidget::webView(void) const
{
	return (m_webView);
}

inline AddressLineEdit* BrowserPageWidget::addressBar(void) const
{
	return (m_addressBar);
}

#endif   // _BrowserPageWidget_h
