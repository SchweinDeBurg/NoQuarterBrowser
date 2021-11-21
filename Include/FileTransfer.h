#if !defined(_FileTransfer_h)
#define _FileTransfer_h

#include <QDebug>
#include <QTableWidgetItem>
#include <QVector>
#include <QWebEngineDownloadItem>

class FileTransfer: public QObject
{
public:
	enum { I_URL, I_PATH, I_SIZE, I_PROGRESS, I_STATE, I_DATETIME, NUM_COLUMNS };

public:
	FileTransfer(QWebEngineDownloadItem* download, const QVector<QTableWidgetItem*>& itemsRow);
	virtual ~FileTransfer(void);

public:
	static const QString& stateToText(QWebEngineDownloadItem::DownloadState state);

private:
	void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
	void onFinished(void);
	void onStateChanged(QWebEngineDownloadItem::DownloadState state);

private:
	QWebEngineDownloadItem* m_download;
	QVector<QTableWidgetItem*> m_itemsRow;
};

#endif   // _FileTransfer_h
