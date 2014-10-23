#include "QtSettingsWindow.h"
#include "Globals.h"

SettingsWindow::SettingsWindow(QWidget *parent)
:QDialog(parent)
{
	setWindowTitle(tr("Settings"));
	setWindowIcon(Globals::instance()->iconOptions());
}

SettingsWindow::~SettingsWindow(void)
{

}
