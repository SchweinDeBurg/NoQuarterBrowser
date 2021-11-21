#include <QContextMenuEvent>
#include <QMenu>
#include <QWebEngineContextMenuData>

#include <CustomWebPage.h>
#include <CustomWebView.h>

CustomWebView::CustomWebView(QWidget* parent):
QWebEngineView(parent)
{
	setPage(new CustomWebPage(QWebEngineProfile::defaultProfile(), this));
}

CustomWebView::~CustomWebView(void)
{
}

void CustomWebView::contextMenuEvent(QContextMenuEvent* event)
{
	QMenu* contextMenu = nullptr;
	if (page()->contextMenuData().linkUrl().isValid())
	{
		contextMenu = new QMenu(this);
		contextMenu->setAttribute(Qt::WA_DeleteOnClose, true);
		page()->action(QWebEnginePage::OpenLinkInNewWindow)->setText(tr("Open Link in New &Window"));
		contextMenu->addAction(page()->action(QWebEnginePage::OpenLinkInNewWindow));
		page()->action(QWebEnginePage::OpenLinkInNewTab)->setText(tr("Open Link in New &Tab"));
		contextMenu->addAction(page()->action(QWebEnginePage::OpenLinkInNewTab));
		page()->action(QWebEnginePage::OpenLinkInNewBackgroundTab)->setText(tr("Open Link in New &Background Tab"));
		contextMenu->addAction(page()->action(QWebEnginePage::OpenLinkInNewBackgroundTab));
		contextMenu->addSeparator();
		page()->action(QWebEnginePage::DownloadLinkToDisk)->setText(tr("Save &Link As..."));
		contextMenu->addAction(page()->action(QWebEnginePage::DownloadLinkToDisk));
		page()->action(QWebEnginePage::CopyLinkToClipboard)->setText(tr("Copy Link &URL"));
		contextMenu->addAction(page()->action(QWebEnginePage::CopyLinkToClipboard));
	}
	else
	{
		contextMenu = page()->createStandardContextMenu();
	}
	if (page()->contextMenuData().selectedText().isEmpty())
	{
		contextMenu->addSeparator();
		page()->action(QWebEnginePage::SavePage)->setText(tr("Save &Page As..."));
		contextMenu->addAction(page()->action(QWebEnginePage::SavePage));
	}
	contextMenu->popup(event->globalPos());
}
