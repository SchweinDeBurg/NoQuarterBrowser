#include <AbstractPageWidget.h>

AbstractPageWidget::~AbstractPageWidget(void)
{
}

BrowserWindow* AbstractPageWidget::parentBrowserWindow(void)
{
	QObject* parentObject = parent();
	while (parentObject != nullptr)
	{
		BrowserWindow* window = dynamic_cast<BrowserWindow*>(parentObject);
		if (window != NULL)
		{
			return (window);
		}
		parentObject = parentObject->parent();
	}
	return (nullptr);
}
