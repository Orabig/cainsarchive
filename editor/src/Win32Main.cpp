#include <windows.h>
#include <QApplication>
#include "QtMainWindow.h"
#include "globals.h"
#include "logger.h"
#include "Settings.h"

int APIENTRY WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)		
//int main(int argc, char *argv[])
{
	//win32 argc doesn't mix well with Qt, not sure why so we generate one as Qt requires the application path.
	//Note this is due to requiring a winmain for access to the HINSTANCE. Breaks cross platform.
	int argc = 1;
	char *argv[1];
	argv[0] = new char[MAX_PATH];
	memset(argv[0], 0, MAX_PATH);
	GetModuleFileNameA(0, argv[0], MAX_PATH);

	Logger::instance()->initialize((LogWindow *)0);
	Settings::instance()->initialize();

	QApplication Application(argc, argv);
//	Application.setStyle(new QCleanlooksStyle);

	Globals::instance()->initialize(hInstance);
	//Settings::instance()->initialize();

	//Initialize Main Window
	MainWindow mwindow(hInstance);

	if(mwindow.initialize())
	{
		return Application.exec();
	}
	else
	{
		return 0;
	}
}
