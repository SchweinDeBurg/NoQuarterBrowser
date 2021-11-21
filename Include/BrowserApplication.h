#if !defined(_BrowserApplication_h)
#define _BrowserApplication_h

#include <QApplication>
#include <QDebug>
#include <QList>

#include <BrowserWindow.h>

class BrowserApplication: public QApplication
{
public:
	BrowserApplication(int& argc, char* argv[]);
	virtual ~BrowserApplication(void);

public:
	BrowserWindow* createBrowserWindow(void);
};

#endif   // _BrowserApplication_h
