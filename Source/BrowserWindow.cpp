#include <QCloseEvent>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QVector>

#include <BrowserApplication.h>
#include <BrowserPageWidget.h>
#include <BrowserWindow.h>
#include <CommonStuff.h>
#include <DownloadsPageWidget.h>
#include <HistoryPageWidget.h>

BrowserWindow::BrowserWindow(QWidget* parent):
QMainWindow(parent)
{
	m_tabWidget = new BrowserTabWidget(this);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addWidget(m_tabWidget);
	mainLayout->setContentsMargins(0, 4, 0, 4);

	QWidget* contentWidget = new QWidget();
	contentWidget->setLayout(mainLayout);
	setCentralWidget(contentWidget);

	setWindowIcon(QIcon(":/Graphics/TangoInetrnetWebBrowser.png"));
	setWindowTitle(tr("NoQuarter Browser"));

	createActions();
	connectActions();
	createMenus();
	setupStatusBar();
}

BrowserWindow::~BrowserWindow(void)
{
	qDebug() << "BrowserWindow::~BrowserWindow()";
}

void BrowserWindow::closeEvent(QCloseEvent* event)
{
	if (m_tabWidget->count() > 1)
	{
		QString confirmtText(tr("You are about to close %1 tabs. Are you sure you want to continue?").arg(m_tabWidget->count()));
		if (QMessageBox::question(this, tr("Close window"), confirmtText) == QMessageBox::No)
		{
			event->ignore();
			return;
		}
	}
	saveSettings();
	event->accept();
}

int BrowserWindow::createBrowserTab(const QString& label, const QUrl& url, bool setCurrent)
{
	int tabIndex = m_tabWidget->createBrowserPage(label, url, setCurrent);
	if (setCurrent)
	{
		m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
	}
	return (tabIndex);
}

int BrowserWindow::createBrowserTabAt(int index, const QString& label, const QUrl& url, bool setCurrent)
{
	int tabIndex = m_tabWidget->createBrowserPageAt(index, label, url, setCurrent);
	if (setCurrent)
	{
		m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
	}
	return (tabIndex);
}

void BrowserWindow::removeTab(int index)
{
	QWidget* somePage = m_tabWidget->widget(index);
	if (m_tabWidget->count() > 1)
	{
		if (auto downloadsPage = dynamic_cast<DownloadsPageWidget*>(somePage); downloadsPage != nullptr)
		{
			downloadsPage->saveSettings();
			m_tabWidget->releaseDownloadsPage();
		}
		else if (auto historyPage = dynamic_cast<HistoryPageWidget*>(somePage); historyPage != nullptr)
		{
			historyPage->saveSettings();
		}
		m_tabWidget->removeTab(index);
	}
	else if (auto browserPage = dynamic_cast<BrowserPageWidget*>(somePage); browserPage != nullptr)
	{
		m_tabWidget->setPageUrl(0, m_blankTabUrl);
	}
}

void BrowserWindow::saveSettings(void)
{
	m_appSettings.beginGroup("BrowserWindow");
	m_appSettings.setValue("Geometry", saveGeometry());
	m_appSettings.setValue("State", saveState());
	m_appSettings.endGroup();
}

bool BrowserWindow::restoreSettings(void)
{
	m_appSettings.beginGroup("BrowserWindow");
	bool hasGeometry = restoreGeometry(m_appSettings.value("Geometry").toByteArray());
	bool hasState = restoreState(m_appSettings.value("State").toByteArray());
	m_appSettings.endGroup();
	return (hasGeometry && hasState);
}

void BrowserWindow::createActions(void)
{
	m_newWindowAction = new QAction(tr("New &Window"), this);
	m_newWindowAction->setShortcut(QKeySequence::New);
	m_newWindowAction->setStatusTip(tr("Create new browser window"));

	m_newTabAction = new QAction(tr("New &Tab"), this);
	m_newTabAction->setShortcut(QKeySequence::AddTab);
	m_newTabAction->setStatusTip(tr("Create new empty tab"));

	m_newBrowserAction = new QAction(tr("New &Browser"), this);
	m_newBrowserAction->setShortcut(tr("Ctrl+B"));
	m_newBrowserAction->setStatusTip(tr("Launch new application instance"));

	m_openFileAction = new QAction(tr("&Open File..."), this);
	m_openFileAction->setShortcut(QKeySequence::Open);
	m_openFileAction->setStatusTip(tr("Open file from local storage"));

	m_closeTabAction = new QAction(tr("&Close Tab"), this);
	m_closeTabAction->setShortcut(QKeySequence::Close);
	m_closeTabAction->setStatusTip(tr("Close the current tab"));

	m_exitAction = new QAction(tr("E&xit"), this);
	m_exitAction->setShortcut(QKeySequence::Quit);
	m_exitAction->setStatusTip(tr("Exit the application"));

	m_nextTabAction = new QAction(tr("&Next Tab"), this);
	m_nextTabAction->setShortcut(QKeySequence::NextChild);
	m_nextTabAction->setStatusTip(tr("Switch to the next tab"));

	m_previousTabAction = new QAction(tr("&Previous Tab"), this);
	m_previousTabAction->setShortcut(QKeySequence::PreviousChild);
	m_previousTabAction->setStatusTip(tr("Switch to the previous tab"));

	m_zoomInAction = new QAction(tr("Zoom &In"), this);
	m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
	m_zoomInAction->setStatusTip(tr("Increase size of text and graphics"));

	m_zoomOutAction = new QAction(tr("Zoom &Out"), this);
	m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
	m_zoomOutAction->setStatusTip(tr("Decrease size of text and graphics"));

	m_zoomResetAction = new QAction(tr("&Zoom Reset"), this);
	m_zoomResetAction->setShortcut(tr("Ctrl+0"));
	m_zoomResetAction->setStatusTip(tr("Reset size of text and graphics"));

	m_refreshAction = new QAction(tr("&Refresh"), this);
	m_refreshAction->setShortcut(QKeySequence::Refresh);
	m_refreshAction->setStatusTip(tr("Reload the web page shown on the current tab"));

	m_downloadsAction = new QAction(tr("&Downloads"), this);
	m_downloadsAction->setStatusTip(tr("Open a tab with the list of downloaded files"));
	m_downloadsAction->setShortcut(tr("Ctrl+D"));

	m_historyAction = new QAction(tr("&History"), this);
	m_historyAction->setStatusTip(tr("Open a tab with the list of visited web pages"));
	m_historyAction->setShortcut(tr("Ctrl+H"));

	m_homePageAction = new QAction(tr("&Home Page"), this);
	m_homePageAction->setStatusTip(tr("Visit project's page on GitHub"));

	m_aboutAction = new QAction(tr("&About..."), this);
	m_aboutAction->setStatusTip(tr("Show information about this application"));

	m_aboutQtAction = new QAction(tr("About &Qt..."), this);
	m_aboutQtAction->setStatusTip(tr("Show information about Qt toolkit"));
}

void BrowserWindow::connectActions(void)
{
	connect(m_newWindowAction, &QAction::triggered, this, &BrowserWindow::onFileNewWindow);
	connect(m_newTabAction, &QAction::triggered, this, &BrowserWindow::onFileNewTab);
	connect(m_newBrowserAction, &QAction::triggered, this, &BrowserWindow::onFileNewBrowser);
	connect(m_openFileAction, &QAction::triggered, this, &BrowserWindow::onFileOpenFile);
	connect(m_closeTabAction, &QAction::triggered, this, &BrowserWindow::onFileCloseTab);
	connect(m_exitAction, &QAction::triggered, this, &BrowserWindow::onFileExit);

	connect(m_nextTabAction, &QAction::triggered, this, &BrowserWindow::onViewNextTab);
	connect(m_previousTabAction, &QAction::triggered, this, &BrowserWindow::onViewPreviousTab);
	connect(m_zoomInAction, &QAction::triggered, this, &BrowserWindow::onViewZoomIn);
	connect(m_zoomOutAction, &QAction::triggered, this, &BrowserWindow::onViewZoomOut);
	connect(m_zoomResetAction, &QAction::triggered, this, &BrowserWindow::onViewZoomReset);
	connect(m_refreshAction, &QAction::triggered, this, &BrowserWindow::onViewRefresh);

	connect(m_downloadsAction, &QAction::triggered, this, &BrowserWindow::onToolsDownloads);
	connect(m_historyAction, &QAction::triggered, this, &BrowserWindow::onToolsHistory);

	connect(m_homePageAction, &QAction::triggered, this, &BrowserWindow::onHelpHomePage);
	connect(m_aboutAction, &QAction::triggered, this, &BrowserWindow::onHelpAbout);
	connect(m_aboutQtAction, &QAction::triggered, this, &BrowserWindow::onHelpAboutQt);

	connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &BrowserWindow::onTabWidgetTabCloseRequested);
}

void BrowserWindow::createMenus(void)
{
	m_fileMenu = menuBar()->addMenu(tr("&File"));
	m_fileMenu->addAction(m_newWindowAction);
	m_fileMenu->addAction(m_newTabAction);
	m_fileMenu->addAction(m_newBrowserAction);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_openFileAction);
	m_fileMenu->addAction(m_closeTabAction);
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_exitAction);

	m_viewMenu = menuBar()->addMenu(tr("&View"));
	m_viewMenu->addAction(m_nextTabAction);
	m_viewMenu->addAction(m_previousTabAction);
	m_viewMenu->addSeparator();
	m_viewMenu->addAction(m_zoomInAction);
	m_viewMenu->addAction(m_zoomOutAction);
	m_viewMenu->addAction(m_zoomResetAction);
	m_viewMenu->addSeparator();
	m_viewMenu->addAction(m_refreshAction);

	m_toolsMenu = menuBar()->addMenu(tr("&Tools"));
	m_toolsMenu->addAction(m_downloadsAction);
	m_toolsMenu->addAction(m_historyAction);

	m_helpMenu = menuBar()->addMenu(tr("&Help"));
	m_helpMenu->addAction(m_homePageAction);
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_aboutAction);
	m_helpMenu->addAction(m_aboutQtAction);
}

void BrowserWindow::setupStatusBar(void)
{
	statusBar()->setSizeGripEnabled(false);

	m_sepZoom = new QFrame(statusBar());
	m_sepZoom->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	statusBar()->addPermanentWidget(m_sepZoom);

	m_textZoom = new QLabel(statusBar());
	m_textZoom->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	m_textZoom->setTextFormat(Qt::PlainText);
	m_textZoom->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	m_textZoom->setIndent(4);
	statusBar()->addPermanentWidget(m_textZoom);

	statusBar()->showMessage(tr("In memory of Led Zeppelin"));
}

void BrowserWindow::onFileNewWindow(void)
{
	BrowserApplication* browserApp = dynamic_cast<BrowserApplication*>(BrowserApplication::instance());
	Q_ASSERT(browserApp != nullptr);
	BrowserWindow* window = browserApp->createBrowserWindow();
	window->show();
	window->createBlankTab();
}

void BrowserWindow::onFileNewTab(void)
{
	createBlankTab();
}

void BrowserWindow::onFileNewBrowser(void)
{
	QProcess::startDetached(QCoreApplication::applicationFilePath());
}

void BrowserWindow::onFileOpenFile(void)
{
	QVector<QString> filtersList =
	{
		tr("All Files (*.*)"),
		tr("HTML Files (*.html *.htm)"),
		tr("Text Files (*.txt)"),
		tr("Image Files (*.bmp *.gif *.jpeg *.jpg *.png *.tiff *.webp *.xpm)"),
		tr("XML Files (*.rdf *.rss *.xhtml *.xml)")
	};
	QString filter = filtersList.at(0);
	for (int i = 1, count = filtersList.count(); i < count; ++i)
	{
		filter += tr(";;");
		filter += filtersList.at(i);
	}

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::homePath(), filter);
	if (!fileName.isEmpty())
	{
		fileName.insert(0, "file://");
		m_tabWidget->createBrowserPage(tr(""), QUrl(fileName));
	}
}

void BrowserWindow::onFileCloseTab(void)
{
	removeTab(m_tabWidget->currentIndex());
}

void BrowserWindow::onFileExit(void)
{
	close();
}

void BrowserWindow::onViewNextTab(void)
{
	m_tabWidget->changeCurrentPage(ChangeDirection::Next);
	m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
}

void BrowserWindow::onViewPreviousTab(void)
{
	m_tabWidget->changeCurrentPage(ChangeDirection::Previous);
	m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
}

void BrowserWindow::onViewZoomIn(void)
{
	m_tabWidget->zoomCurrentPage(ZoomDirection::In);
	m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
}

void BrowserWindow::onViewZoomOut(void)
{
	m_tabWidget->zoomCurrentPage(ZoomDirection::Out);
	m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
}

void BrowserWindow::onViewZoomReset(void)
{
	m_tabWidget->zoomCurrentPage(ZoomDirection::Reset);
	m_textZoom->setText(QString(tr("Zoom: %1%").arg(m_tabWidget->currentPageZoomPercent())));
}

void BrowserWindow::onViewRefresh(void)
{
	m_tabWidget->refreshCurrentPage();
}

void BrowserWindow::onToolsDownloads(void)
{
	m_tabWidget->createDownloadsPage();
}

void BrowserWindow::onToolsHistory(void)
{
	m_tabWidget->createHistoryPage();
}

void BrowserWindow::onHelpHomePage(void)
{
	m_tabWidget->createBrowserPage(tr(""), QUrl("https://github.com/SchweinDeBurg/"));
}

void BrowserWindow::onHelpAbout(void)
{
	QString aboutText("<strong>NoQuarter Browser &mdash; version 0.1.0</strong><br><br>");
	aboutText += tr("Written by Elijah Zarezky for study Qt programming<br>");
	aboutText += tr("Named in honor of the fine song by Led Zeppelin");
	QMessageBox::about(this, tr("About NoQuarter Browser"), aboutText);
}

void BrowserWindow::onHelpAboutQt(void)
{
	QMessageBox::aboutQt(this, tr("About Qt"));
}

void BrowserWindow::onTabWidgetTabCloseRequested(int index)
{
	removeTab(index);
}

QSettings BrowserWindow::m_appSettings("Zarezky.spb.ru", "NoQuarterBrowser");
QUrl BrowserWindow::m_blankTabUrl("qrc:/HTML/NewTabPage.html");
QHash<QString, QDateTime> BrowserWindow::m_urlsHistory;
