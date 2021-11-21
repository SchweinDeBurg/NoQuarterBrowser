#include <QDateTime>

#include <FileTransfer.h>

FileTransfer::FileTransfer(QWebEngineDownloadItem* download, const QVector<QTableWidgetItem*>& itemsRow):
m_download(download),
m_itemsRow(itemsRow)
{
	connect(m_download, &QWebEngineDownloadItem::downloadProgress, this, &FileTransfer::onDownloadProgress);
	connect(m_download, &QWebEngineDownloadItem::finished, this, &FileTransfer::onFinished);
	connect(m_download, &QWebEngineDownloadItem::stateChanged, this, &FileTransfer::onStateChanged);
}

FileTransfer::~FileTransfer(void)
{
}

const QString& FileTransfer::stateToText(QWebEngineDownloadItem::DownloadState state)
{
	static QMap<QWebEngineDownloadItem::DownloadState, QString> statesMap =
	{
		std::make_pair(QWebEngineDownloadItem::DownloadRequested, tr("Requested")),
		std::make_pair(QWebEngineDownloadItem::DownloadInProgress, tr("In Progress")),
		std::make_pair(QWebEngineDownloadItem::DownloadCompleted, tr("Completed")),
		std::make_pair(QWebEngineDownloadItem::DownloadCancelled, tr("Cancelled")),
		std::make_pair(QWebEngineDownloadItem::DownloadInterrupted, tr("Interrupted"))
	};
	return (statesMap[state]);
}

void FileTransfer::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	m_itemsRow[I_SIZE]->setText(QString("%L1").arg(bytesTotal));
	m_itemsRow[I_PROGRESS]->setText(QString("%1%").arg((bytesReceived * 100) / bytesTotal));
	m_itemsRow[I_DATETIME]->setText(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"));
}

void FileTransfer::onFinished(void)
{
	m_download = nullptr;
}

void FileTransfer::onStateChanged(QWebEngineDownloadItem::DownloadState state)
{
	m_itemsRow[I_STATE]->setText(stateToText(state));
}
