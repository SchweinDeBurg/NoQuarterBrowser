#if !defined(_BrowserWindow_h)
#define _BrowserWindow_h

#include <QAction>
#include <QDateTime>
#include <QDebug>
#include <QHash>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QUrl>

#include <BrowserTabWidget.h>

class BrowserWindow: public QMainWindow
{
public:
	BrowserWindow(QWidget* parent = nullptr);
	virtual ~BrowserWindow(void);

protected:
	virtual void closeEvent(QCloseEvent* event);

public:
	int createBrowserTab(const QString& label, const QUrl& url = QUrl(), bool setCurrent = true);
	int createBlankTab(bool setCurrent = true);
	int createBrowserTabAt(int index, const QString& label, const QUrl& url = QUrl(), bool setCurrent = true);
	int createBlankTabAt(int index, bool setCurrent = true);
	void removeTab(int index);
	void saveSettings(void);
	bool restoreSettings(void);

private:
	void createActions(void);
	void connectActions(void);
	void createMenus(void);
	void setupStatusBar(void);

private:
	void onFileNewWindow(void);
	void onFileNewTab(void);
	void onFileNewBrowser(void);
	void onFileOpenFile(void);
	void onFileCloseTab(void);
	void onFileExit(void);
	void onViewNextTab(void);
	void onViewPreviousTab(void);
	void onViewZoomIn(void);
	void onViewZoomOut(void);
	void onViewZoomReset(void);
	void onViewRefresh(void);
	void onToolsDownloads(void);
	void onToolsHistory(void);
	void onToolsHighlightAllLinks(void);
	void onHelpHomePage(void);
	void onHelpAbout(void);
	void onHelpAboutQt(void);

private:
	void onTabWidgetTabCloseRequested(int index);

private:
	QAction* m_newWindowAction;
	QAction* m_newTabAction;
	QAction* m_newBrowserAction;
	QAction* m_openFileAction;
	QAction* m_closeTabAction;
	QAction* m_exitAction;
	QAction* m_nextTabAction;
	QAction* m_previousTabAction;
	QAction* m_zoomInAction;
	QAction* m_zoomOutAction;
	QAction* m_zoomResetAction;
	QAction* m_refreshAction;
	QAction* m_downloadsAction;
	QAction* m_historyAction;
	QAction* m_highlightAllLinksAction;
	QAction* m_homePageAction;
	QAction* m_aboutAction;
	QAction* m_aboutQtAction;

private:
	QMenu* m_fileMenu;
	QMenu* m_viewMenu;
	QMenu* m_toolsMenu;
	QMenu* m_helpMenu;

public:
	BrowserTabWidget* tabWidget(void) const;
	static QSettings& appSettings(void);
	static const QUrl& blankTabUrl(void);
	static QHash<QString, QDateTime>& urlsHistory(void);

private:
	BrowserTabWidget* m_tabWidget;
	QFrame* m_sepZoom;
	QLabel* m_textZoom;
	static QSettings m_appSettings;
	static QUrl m_blankTabUrl;
	static QHash<QString, QDateTime> m_urlsHistory;
};

inline int BrowserWindow::createBlankTab(bool setCurrent)
{
	return (createBrowserTab(tr("NoQuarter"), m_blankTabUrl, setCurrent));
}

inline int BrowserWindow::createBlankTabAt(int index, bool setCurrent)
{
	return (createBrowserTabAt(index, tr("NoQuarter"), m_blankTabUrl, setCurrent));
}

inline BrowserTabWidget* BrowserWindow::tabWidget(void) const
{
	return (m_tabWidget);
}

inline QSettings& BrowserWindow::appSettings(void)
{
	return (m_appSettings);
}

inline const QUrl& BrowserWindow::blankTabUrl(void)
{
	return (m_blankTabUrl);
}

inline QHash<QString, QDateTime>& BrowserWindow::urlsHistory(void)
{
	return (m_urlsHistory);
}

#endif   // _BrowserWindow_h
