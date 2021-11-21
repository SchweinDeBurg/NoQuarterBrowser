#include <BrowserApplication.h>
#include <BrowserPageWidget.h>
#include <CustomWebPage.h>

CustomWebPage::CustomWebPage(QObject* parent):
QWebEnginePage(parent)
{
}

CustomWebPage::CustomWebPage(QWebEngineProfile* profile, QObject* parent):
QWebEnginePage(profile, parent)
{
}

CustomWebPage::~CustomWebPage(void)
{
}

QWebEnginePage* CustomWebPage::createWindow(QWebEnginePage::WebWindowType type)
{
	BrowserApplication* browserApp = dynamic_cast<BrowserApplication*>(BrowserApplication::instance());
	Q_ASSERT(browserApp != nullptr);

	switch (type)
	{
		case QWebEnginePage::WebBrowserWindow:
		{
			BrowserWindow* window = browserApp->createBrowserWindow();
			window->show();
			int tabIndex = window->createBlankTab();
			BrowserPageWidget* pageWidget = dynamic_cast<BrowserPageWidget*>(window->tabWidget()->widget(tabIndex));
			Q_ASSERT(pageWidget != nullptr);
			return (pageWidget->webView()->page());
		}
		case QWebEnginePage::WebBrowserTab:
		{
			BrowserWindow* window = parentBrowserWindow();
			if (window != nullptr)
			{
				int tabIndex = window->createBlankTab();
				BrowserPageWidget* pageWidget = dynamic_cast<BrowserPageWidget*>(window->tabWidget()->widget(tabIndex));
				Q_ASSERT(pageWidget != nullptr);
				return (pageWidget->webView()->page());
			}
			break;
		}
		case QWebEnginePage::WebBrowserBackgroundTab:
		{
			BrowserWindow* window = parentBrowserWindow();
			if (window != nullptr)
			{
				int tabIndex = window->createBlankTab(false);
				BrowserPageWidget* pageWidget = dynamic_cast<BrowserPageWidget*>(window->tabWidget()->widget(tabIndex));
				Q_ASSERT(pageWidget != nullptr);
				return (pageWidget->webView()->page());
			}
			break;
		}
	}

	return (QWebEnginePage::createWindow(type));
}

BrowserWindow* CustomWebPage::parentBrowserWindow(void)
{
	QObject* parentObject = parent();
	while (parentObject != nullptr)
	{
		BrowserWindow* window = dynamic_cast<BrowserWindow*>(parentObject);
		if (window != NULL)
		{
			return (window);
		}
		parentObject = parentObject->parent();
	}
	return (nullptr);
}
