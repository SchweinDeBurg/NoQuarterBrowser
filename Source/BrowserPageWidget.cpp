#include <QApplication>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebEngineProfile>

#include <BrowserPageWidget.h>
#include <BrowserWindow.h>
#include <DownloadsPageWidget.h>

BrowserPageWidget::BrowserPageWidget(BrowserTabWidget* tabWidget, QWidget* parent):
AbstractPageWidget(tabWidget, parent)
{
	m_viewFrame = new QFrame(this);
	m_viewFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);

	m_webView = new CustomWebView(this);

	m_buttonsBar = new QToolBar(this);
	m_buttonsBar->addAction(m_webView->pageAction(QWebEnginePage::Back));
	m_buttonsBar->addAction(m_webView->pageAction(QWebEnginePage::Forward));
	m_buttonsBar->addAction(m_webView->pageAction(QWebEnginePage::Reload));
	m_buttonsBar->addAction(m_webView->pageAction(QWebEnginePage::Stop));

	m_addressBar = new AddressLineEdit(m_buttonsBar);
	m_addressBar->setSizePolicy(QSizePolicy::Expanding, m_addressBar->sizePolicy().verticalPolicy());
	m_buttonsBar->addWidget(m_addressBar);

	QVBoxLayout* pageLayout = new QVBoxLayout();
	pageLayout->setMenuBar(m_buttonsBar);
	pageLayout->addWidget(m_viewFrame);
	pageLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(pageLayout);

	QVBoxLayout* viewLayout = new QVBoxLayout();
	viewLayout->addWidget(m_webView);
	viewLayout->setContentsMargins(0, 0, 0, 0);
	m_viewFrame->setLayout(viewLayout);

	connect(m_addressBar, &QLineEdit::returnPressed, this, &BrowserPageWidget::onAddressBarReturnPressed);
	connect(m_webView, &QWebEngineView::loadProgress, this, &BrowserPageWidget::onWebViewLoadProgress);
	connect(m_webView, &QWebEngineView::loadFinished, this, &BrowserPageWidget::onWebViewLoadFinished);
	connect(m_webView->page()->profile(), &QWebEngineProfile::downloadRequested, this, &BrowserPageWidget::onWebViewDownloadRequested);

	m_focusAddressBarAction = new QAction(this);
	m_focusAddressBarAction->setShortcut(tr("Ctrl+L"));
	addAction(m_focusAddressBarAction);
	connect(m_focusAddressBarAction, &QAction::triggered, this, &BrowserPageWidget::onFocusAddressBar);

	QFile resourceFile(":/Scripts/jquery-3.6.0-min.js");
	if (resourceFile.open(QIODevice::ReadOnly))
	{
		m_dzhejKwieri = resourceFile.readAll();
		m_dzhejKwieri.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
		resourceFile.close();
	}
}

BrowserPageWidget::~BrowserPageWidget(void)
{
}

void BrowserPageWidget::zoomWebView(ZoomDirection direction)
{
	unsigned int zoomPercent = webViewZoomPercent();
	if (direction == ZoomDirection::In && zoomPercent < 500)
	{
		m_webView->setZoomFactor(qreal(zoomPercent + 25) / qreal(100.0));
	}
	else if (direction == ZoomDirection::Out && zoomPercent > 25)
	{
		m_webView->setZoomFactor(qreal(zoomPercent - 25) / qreal(100.0));
	}
	else if (direction == ZoomDirection::Reset && zoomPercent != 100)
	{
		m_webView->setZoomFactor(1.0);
	}
}

unsigned int BrowserPageWidget::webViewZoomPercent(void) const
{
	unsigned int zoomPercent = static_cast<unsigned int>(m_webView->zoomFactor() * qreal(100.0));
	unsigned int divReminder = zoomPercent % 25;
	return (divReminder != 0 ? zoomPercent + 25 - divReminder : zoomPercent);
}

void BrowserPageWidget::onAddressBarReturnPressed(void)
{
	m_webView->load(QUrl::fromUserInput(m_addressBar->text()));
	m_webView->setFocus();
}

void BrowserPageWidget::onWebViewLoadProgress(int progress)
{
	m_tabWidget->setTabText(this, QString(tr("Loading: %1%").arg(progress)));
}

void BrowserPageWidget::onWebViewLoadFinished(bool ok)
{
	QString urlString = m_webView->url().toString();
	m_addressBar->setText(urlString);
	m_tabWidget->setTabText(this, m_webView->title());
	if (urlString != BrowserWindow::blankTabUrl().toString())
	{
		BrowserWindow::urlsHistory().insert(urlString, QDateTime::currentDateTime());
	}
	else
	{
		// https://stackoverflow.com/a/51807268
		QTimer::singleShot(0, m_addressBar, &QLineEdit::selectAll);
	}
	m_webView->page()->runJavaScript(m_dzhejKwieri);
}

void BrowserPageWidget::onWebViewDownloadRequested(QWebEngineDownloadItem* download)
{
	if (download->savePageFormat() != QWebEngineDownloadItem::UnknownSaveFormat)
	{
	}
	else
	{
		// this is not a request for downloading a complete web page
		QString destDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
		QString destFile = download->url().fileName();
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Link"), QString(destDir + "/" + destFile));
		if (!fileName.isEmpty())
		{
			download->setPath(fileName);
			m_tabWidget->createDownloadsPage();
			DownloadsPageWidget* downloadsPage = dynamic_cast<DownloadsPageWidget*>(m_tabWidget->downloadsPage());
			Q_ASSERT(downloadsPage != nullptr);
			downloadsPage->createFileTransfer(download);
			download->accept();
		}
	}
}

void BrowserPageWidget::onFocusAddressBar(void)
{
	focusAddressBar(Qt::OtherFocusReason);
}
