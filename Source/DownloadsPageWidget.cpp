#include <QDateTime>
#include <QHeaderView>
#include <QMap>
#include <QSettings>
#include <QStringList>
#include <QVBoxLayout>

#include <DownloadsPageWidget.h>

DownloadsPageWidget::DownloadsPageWidget(BrowserTabWidget* tabWidget, QWidget* parent):
AbstractPageWidget(tabWidget, parent)
{
	m_tableView = new QTableWidget(this);
	m_tableView->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_tableView->setShowGrid(true);
	m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tableView->setColumnCount(FileTransfer::NUM_COLUMNS);
	QStringList headersList = { tr("URL"), tr("Path"), tr("Size"), tr("Progreess"), tr("State"), tr("Date") };
	m_tableView->setHorizontalHeaderLabels(headersList);
	m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	m_tableView->horizontalHeader()->setHighlightSections(false);

	QVBoxLayout* viewLayout = new QVBoxLayout();
	viewLayout->addWidget(m_tableView);
	viewLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(viewLayout);
}

DownloadsPageWidget::~DownloadsPageWidget(void)
{
}

void DownloadsPageWidget::createFileTransfer(QWebEngineDownloadItem* download)
{
	QVector<QTableWidgetItem*> itemsRow =
	{
		new QTableWidgetItem(download->url().url()),
		new QTableWidgetItem(download->path()),
		new QTableWidgetItem(QString("%L1").arg(download->totalBytes())),
		new QTableWidgetItem("0%"),
		new QTableWidgetItem(FileTransfer::stateToText(download->state())),
		new QTableWidgetItem(QDateTime::currentDateTime().toString("dd.MM.yyyy HH:mm:ss"))
	};
	itemsRow[FileTransfer::I_URL]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	itemsRow[FileTransfer::I_PATH]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	itemsRow[FileTransfer::I_SIZE]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	itemsRow[FileTransfer::I_PROGRESS]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	itemsRow[FileTransfer::I_STATE]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
	itemsRow[FileTransfer::I_DATETIME]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

	m_tableView->insertRow(0);
	for (int column = 0; column < FileTransfer::NUM_COLUMNS; ++column)
	{
		m_tableView->setItem(0, column, itemsRow[column]);
	}
	m_tableView->selectRow(0);
	FileTransfer* newTransfer = new FileTransfer(download, itemsRow);
	m_transfersList.insert(0, newTransfer);
}

void DownloadsPageWidget::saveSettings(void)
{
	QSettings& appSettings = BrowserWindow::appSettings();
	appSettings.beginGroup("DownloadsPage");
	appSettings.setValue("HeaderState", m_tableView->horizontalHeader()->saveState());
	appSettings.endGroup();
}

bool DownloadsPageWidget::restoreSettings(void)
{
	QSettings& appSettings = BrowserWindow::appSettings();
	appSettings.beginGroup("DownloadsPage");
	bool hasState = m_tableView->horizontalHeader()->restoreState(appSettings.value("HeaderState").toByteArray());
	appSettings.endGroup();
	return (hasState);
}
