#if !defined(_DownloadsPageWidget_h)
#define _DownloadsPageWidget_h

#include <QDebug>
#include <QList>
#include <QTableWidget>

#include <AbstractPageWidget.h>
#include <FileTransfer.h>

class DownloadsPageWidget: public AbstractPageWidget
{
public:
	DownloadsPageWidget(BrowserTabWidget* tabWidget, QWidget* parent = nullptr);
	virtual ~DownloadsPageWidget(void);

public:
	void createFileTransfer(QWebEngineDownloadItem* download);
	void saveSettings(void);
	bool restoreSettings(void);

private:
	QList<FileTransfer*> m_transfersList;
	QTableWidget* m_tableView;
};

#endif   // _DownloadsPageWidget_h
