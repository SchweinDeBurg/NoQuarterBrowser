#include <QDesktopWidget>

#include <BrowserApplication.h>

BrowserApplication::BrowserApplication(int& argc, char* argv[]):
QApplication(argc, argv)
{
}

BrowserApplication::~BrowserApplication(void)
{
}

BrowserWindow* BrowserApplication::createBrowserWindow(void)
{
	BrowserWindow* window = new BrowserWindow();
	if (!window->restoreSettings())
	{
		QRect screenGeometry = QApplication::desktop()->screenGeometry();
		window->resize(screenGeometry.width() / 2, screenGeometry.height() / 2);
		int x = (screenGeometry.width() - window->width()) / 2;
		int y = (screenGeometry.height() - window->height()) / 2;
		window->move(x, y);
	}
	return (window);
}
