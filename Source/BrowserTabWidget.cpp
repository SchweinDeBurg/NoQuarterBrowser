#include <QApplication>

#include <BrowserPageWidget.h>
#include <BrowserTabWidget.h>
#include <BrowserWindow.h>
#include <DownloadsPageWidget.h>
#include <HistoryPageWidget.h>

BrowserTabWidget::BrowserTabWidget(QWidget* parent):
QTabWidget(parent),
m_downloadsPage(nullptr)
{
	setTabsClosable(true);
	setUsesScrollButtons(true);
	setMovable(true);
	setDocumentMode(false);
}

BrowserTabWidget::~BrowserTabWidget(void)
{
}

int BrowserTabWidget::createBrowserPage(const QString& label, const QUrl& url, bool setCurrent)
{
	return (createBrowserPage(url, setCurrent, &BrowserTabWidget::createTab, label));
}

int BrowserTabWidget::createBrowserPageAt(int index, const QString& label, const QUrl& url, bool setCurrent)
{
	return (createBrowserPage(url, setCurrent, &BrowserTabWidget::createTabAt, index, label));
}

int BrowserTabWidget::createDownloadsPage(bool setCurrent)
{
	const QString title(tr("Downloads"));
	return (createDownloadsPage(setCurrent, &BrowserTabWidget::createTab, title));
}

int BrowserTabWidget::createDownloadsPageAt(int index, bool setCurrent)
{
	const QString title(tr("Downloads"));
	return (createDownloadsPage(setCurrent, &BrowserTabWidget::createTabAt, index, title));
}

int BrowserTabWidget::createHistoryPage(bool setCurrent)
{
	const QString title(tr("History"));
	return (createHistoryPage(setCurrent, &BrowserTabWidget::createTab, title));
}

int BrowserTabWidget::createHistoryPageAt(int index, bool setCurrent)
{
	const QString title(tr("History"));
	return (createHistoryPage(setCurrent, &BrowserTabWidget::createTabAt, index, title));
}

bool BrowserTabWidget::setPageUrl(int index, const QUrl& url)
{
	if (index >= 0 && index < count())
	{
		if (auto browserPage = dynamic_cast<BrowserPageWidget*>(widget(index)); browserPage != nullptr)
		{
			browserPage->setWebViewUrl(url);
			return (true);
		}
	}
	return (false);
}

bool BrowserTabWidget::setTabText(QWidget* pageWidget, const QString& label)
{
	int tabIndex = indexOf(pageWidget);
	if (tabIndex < 0)
	{
		return (false);
	}
	QTabWidget::setTabText(tabIndex, label);
	return (true);
}

bool BrowserTabWidget::zoomCurrentPage(ZoomDirection direction)
{
	if (auto browserPage = dynamic_cast<BrowserPageWidget*>(currentWidget()); browserPage != nullptr)
	{
		browserPage->zoomWebView(direction);
		return (true);
	}
	return (false);
}

void BrowserTabWidget::changeCurrentPage(ChangeDirection direction)
{
	int pagesCount = count();
	if (pagesCount > 1)
	{
		int curIndex = currentIndex();
		if (direction == ChangeDirection::Next && ++curIndex == pagesCount)
		{
			curIndex = 0;
		}
		else if (direction == ChangeDirection::Previous && --curIndex < 0)
		{
			curIndex = pagesCount - 1;
		}
		setCurrentIndex(curIndex);
	}
}

bool BrowserTabWidget::refreshCurrentPage(void)
{
	if (auto browserPage = dynamic_cast<BrowserPageWidget*>(currentWidget()); browserPage != nullptr)
	{
		browserPage->refreshWebView();
		return (true);
	}
	else if (auto historyPage = dynamic_cast<HistoryPageWidget*>(currentWidget()); historyPage != nullptr)
	{
		historyPage->refreshTableView();
		return (true);
	}
	return (false);
}

unsigned int BrowserTabWidget::currentPageZoomPercent(void) const
{
	BrowserPageWidget* browserPage = dynamic_cast<BrowserPageWidget*>(currentWidget());
	return (browserPage != nullptr ? browserPage->webViewZoomPercent() : 100);
}

int BrowserTabWidget::createTab(QWidget* page, const QString& label)
{
	return (QTabWidget::addTab(page, label));
}

int BrowserTabWidget::createTabAt(QWidget* page, int& index, const QString& label)
{
	return (QTabWidget::insertTab(index, page, label));
}

template<typename... Args>
int BrowserTabWidget::createBrowserPage(const QUrl& url, bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs)
{
	setUpdatesEnabled(false);
	BrowserPageWidget* browserPage = new BrowserPageWidget(this);
	browserPage->setWebViewUrl(url);
	int tabIndex = (this->*createFunc)(browserPage, std::forward<Args>(createArgs)...);
	if (setCurrent)
	{
		setCurrentIndex(tabIndex);
		browserPage->focusAddressBar(Qt::OtherFocusReason);
	}
	setUpdatesEnabled(true);
	return (tabIndex);
}

template<typename... Args>
int BrowserTabWidget::createDownloadsPage(bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs)
{
	if (m_downloadsPage != nullptr)
	{
		int tabIndex = indexOf(m_downloadsPage);
		if (setCurrent)
		{
			setCurrentIndex(tabIndex);
		}
		return (tabIndex);
	}
	else
	{
		for (QWidget* widget : QApplication::topLevelWidgets())
		{
			if (auto window = dynamic_cast<BrowserWindow*>(widget); window != nullptr)
			{
				if (window->tabWidget()->m_downloadsPage != nullptr)
				{
					QApplication::setActiveWindow(window);
					int tabIndex = window->tabWidget()->indexOf(window->tabWidget()->m_downloadsPage);
					if (setCurrent)
					{
						window->tabWidget()->setCurrentIndex(tabIndex);
					}
					return (tabIndex);
				}
			}
		}
		setUpdatesEnabled(false);
		m_downloadsPage = new DownloadsPageWidget(this);
		int tabIndex = (this->*createFunc)(m_downloadsPage, std::forward<Args>(createArgs)...);
		if (setCurrent)
		{
			setCurrentIndex(tabIndex);
		}
		dynamic_cast<DownloadsPageWidget*>(m_downloadsPage)->restoreSettings();
		setUpdatesEnabled(true);
		return (tabIndex);
	}
}

template<typename... Args>
int BrowserTabWidget::createHistoryPage(bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs)
{
	HistoryPageWidget* historyPage = nullptr;
	int tabIndex = -1;

	for (int index = 0, pagesCount = count(); index < pagesCount && tabIndex < 0; ++index)
	{
		if ((historyPage = dynamic_cast<HistoryPageWidget*>(widget(index))) != nullptr)
		{
			tabIndex = index;
		}
	}

	if (tabIndex < 0)
	{
		setUpdatesEnabled(false);
		historyPage = new HistoryPageWidget(this);
		tabIndex = (this->*createFunc)(historyPage, std::forward<Args>(createArgs)...);
		historyPage->restoreSettings();
		historyPage->refreshTableView();
		setUpdatesEnabled(true);
	}

	if (setCurrent)
	{
		setCurrentIndex(tabIndex);
	}
	return (tabIndex);
}
