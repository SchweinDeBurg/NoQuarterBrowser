#if !defined(_BrowserTabWidget_h)
#define _BrowserTabWidget_h

#include <QDebug>
#include <QTabWidget>
#include <QUrl>

#include <CommonStuff.h>

class BrowserTabWidget: public QTabWidget
{
public:
	BrowserTabWidget(QWidget* parent = nullptr);
	virtual ~BrowserTabWidget(void);

public:
	int createBrowserPage(const QString& label, const QUrl& url = QUrl(), bool setCurrent = true);
	int createBrowserPageAt(int index, const QString& label, const QUrl& url = QUrl(), bool setCurrent = true);
	int createDownloadsPage(bool setCurrent = true);
	int createDownloadsPageAt(int index, bool setCurrent = true);
	int createHistoryPage(bool setCurrent = true);
	int createHistoryPageAt(int index, bool setCurrent = true);
	void releaseDownloadsPage(void);
	bool setPageUrl(int index, const QUrl& url);
	bool setTabText(QWidget* pageWidget, const QString& label);
	bool zoomCurrentPage(ZoomDirection direction);
	void changeCurrentPage(ChangeDirection direction);
	bool refreshCurrentPage(void);

public:
	unsigned int currentPageZoomPercent(void) const;
	QWidget* downloadsPage(void) const;

private:
	int createTab(QWidget* page, const QString& label);
	int createTabAt(QWidget* page, int& index, const QString& label);
	template<typename... Args>
	int createBrowserPage(const QUrl& url, bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs);
	template<typename... Args>
	int createDownloadsPage(bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs);
	template<typename... Args>
	int createHistoryPage(bool setCurrent, int (BrowserTabWidget::*createFunc)(QWidget*, Args...), Args&&... createArgs);

private:
	QWidget* m_downloadsPage;
};

inline void BrowserTabWidget::releaseDownloadsPage(void)
{
	m_downloadsPage = nullptr;
}

inline QWidget* BrowserTabWidget::downloadsPage(void) const
{
	return (m_downloadsPage);
}

#endif   // _BrowserTabWidget_h
