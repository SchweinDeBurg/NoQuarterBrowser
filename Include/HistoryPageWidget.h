#if !defined(_HistoryPageWidget_h)
#define _HistoryPageWidget_h

#include <QDebug>
#include <QTableWidget>

#include <AbstractPageWidget.h>

class HistoryPageWidget: public AbstractPageWidget
{
private:
	enum { I_URL, I_DATETIME, NUM_COLUMNS };

public:
	HistoryPageWidget(BrowserTabWidget* tabWidget, QWidget* parent = nullptr);
	virtual ~HistoryPageWidget(void);

public:
	void refreshTableView(void);
	void saveSettings(void);
	bool restoreSettings(void);

private:
	void onTableViewCustomContextMenuRequested(const QPoint& pos);

private:
	void onOpenInNewWindow(void);
	void onOpenInNewTab(void);

private:
	QAction* m_openInNewWindowAction;
	QAction* m_openInNewTabAction;

private:
	QTableWidget* m_tableView;
};

#endif   // _HistoryPageWidget_h
