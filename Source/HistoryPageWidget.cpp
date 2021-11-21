#include <QDateTime>
#include <QHeaderView>
#include <QList>
#include <QPair>
#include <QSettings>
#include <QVBoxLayout>

#include <algorithm>

#include <BrowserWindow.h>
#include <HistoryPageWidget.h>

HistoryPageWidget::HistoryPageWidget(BrowserTabWidget* tabWidget, QWidget* parent):
AbstractPageWidget(tabWidget, parent)
{
	m_tableView = new QTableWidget(this);
	m_tableView->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_tableView->setShowGrid(true);
	m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
	m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	m_tableView->setColumnCount(NUM_COLUMNS);
	QStringList headersList = { tr("URL"), tr("Date") };
	m_tableView->setHorizontalHeaderLabels(headersList);
	m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	m_tableView->horizontalHeader()->setHighlightSections(false);

	QVBoxLayout* viewLayout = new QVBoxLayout();
	viewLayout->addWidget(m_tableView);
	viewLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(viewLayout);
}

HistoryPageWidget::~HistoryPageWidget(void)
{
}

void HistoryPageWidget::refreshTableView(void)
{
	QHash<QString, QDateTime>& urlsHistory = BrowserWindow::urlsHistory();
	QList<QPair<QString, QDateTime>> urlsList;
	for (auto key : urlsHistory.keys())
	{
		urlsList.append(QPair<QString, QDateTime>(key, urlsHistory[key]));
	}
	std::sort(urlsList.begin(), urlsList.end(), [](auto lhs, auto rhs)
	{
		return (lhs.second > rhs.second);
	});

	setUpdatesEnabled(false);
	m_tableView->setRowCount(0);
	for (auto curPair : urlsList)
	{
		QVector<QTableWidgetItem*> itemsRow =
		{
			new QTableWidgetItem(curPair.first),
			new QTableWidgetItem(curPair.second.toString("dd.MM.yyyy HH:mm:ss"))
		};
		itemsRow[I_URL]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		itemsRow[I_DATETIME]->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		m_tableView->insertRow(m_tableView->rowCount());
		for (int row = m_tableView->rowCount() - 1, column = 0; column < NUM_COLUMNS; ++column)
		{
			m_tableView->setItem(row, column, itemsRow[column]);
		}
	}
	if (m_tableView->rowCount() > 0)
	{
		m_tableView->selectRow(0);
	}
	setUpdatesEnabled(true);
}

void HistoryPageWidget::saveSettings(void)
{
	QSettings& appSettings = BrowserWindow::appSettings();
	appSettings.beginGroup("HistoryPage");
	appSettings.setValue("HeaderState", m_tableView->horizontalHeader()->saveState());
	appSettings.endGroup();
}

bool HistoryPageWidget::restoreSettings(void)
{
	QSettings& appSettings = BrowserWindow::appSettings();
	appSettings.beginGroup("HistoryPage");
	bool hasState = m_tableView->horizontalHeader()->restoreState(appSettings.value("HeaderState").toByteArray());
	appSettings.endGroup();
	return (hasState);
}
