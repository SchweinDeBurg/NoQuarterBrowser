#include <QDateTime>
#include <QHeaderView>
#include <QList>
#include <QMenu>
#include <QPair>
#include <QSettings>
#include <QVBoxLayout>

#include <algorithm>

#include <BrowserApplication.h>
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
	m_tableView->setContextMenuPolicy(Qt::CustomContextMenu);

	QVBoxLayout* viewLayout = new QVBoxLayout();
	viewLayout->addWidget(m_tableView);
	viewLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(viewLayout);

	m_openInNewWindowAction = new QAction(tr("Open URL in New &Window"), this);
	connect(m_openInNewWindowAction, &QAction::triggered, this, &HistoryPageWidget::onOpenInNewWindow);
	m_openInNewTabAction = new QAction(tr("Open URL in New &Tab"), this);
	connect(m_openInNewTabAction, &QAction::triggered, this, &HistoryPageWidget::onOpenInNewTab);

	connect(m_tableView, &QTableWidget::customContextMenuRequested, this, &HistoryPageWidget::onTableViewCustomContextMenuRequested);
}

HistoryPageWidget::~HistoryPageWidget(void)
{
}

void HistoryPageWidget::refreshTableView(void)
{
	m_tableView->setRowCount(0);
	QHash<QString, QDateTime>& urlsHistory = BrowserWindow::urlsHistory();
	if (urlsHistory.count() > 0)
	{
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

void HistoryPageWidget::onTableViewCustomContextMenuRequested(const QPoint& pos)
{
	if (QTableWidgetItem* item = m_tableView->itemAt(pos); item != nullptr)
	{
		QMenu* contextMenu = new QMenu(this);
		contextMenu->setAttribute(Qt::WA_DeleteOnClose, true);
		contextMenu->addAction(m_openInNewWindowAction);
		contextMenu->addAction(m_openInNewTabAction);
		contextMenu->popup(QCursor::pos());
	}
}

void HistoryPageWidget::onOpenInNewWindow(void)
{
	if (auto browserApp = dynamic_cast<BrowserApplication*>(BrowserApplication::instance()); browserApp != nullptr)
	{
		BrowserWindow* window = browserApp->createBrowserWindow();
		window->show();
		QApplication::setActiveWindow(window);
		QUrl url(m_tableView->item(m_tableView->selectedItems().first()->row(), I_URL)->text());
		window->createBrowserTab(tr("NoQuarter"), url);
	}
}

void HistoryPageWidget::onOpenInNewTab(void)
{
	QUrl url(m_tableView->item(m_tableView->selectedItems().first()->row(), I_URL)->text());
	m_tabWidget->createBrowserPage(tr("NoQuarter"), url);
}
