#include <QDebug>
#include <QLocale>
#include <QtWebEngine>

#include <BrowserApplication.h>
#include <BrowserWindow.h>

int main(int argc, char* argv[])
{
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedKingdom));
	BrowserApplication browserApp(argc, argv);
	browserApp.setStyleSheet("QStatusBar::item { border: 0px; }");
	QtWebEngine::initialize();

	BrowserWindow* window = browserApp.createBrowserWindow();
	window->show();
	window->createBlankTab();

	return (browserApp.exec());
}
