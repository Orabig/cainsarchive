#ifndef _QTSETTINGSWINDOW_H_
#define _QTSETTINGSWINDOW_H_
#include <QDialog>

class SettingsWindow : public QDialog
{
	Q_OBJECT

	private:
		
	public:
		SettingsWindow(QWidget *parent);
		~SettingsWindow(void);
};

#endif //#ifndef _QTSETTINGSWINDOW_H_
