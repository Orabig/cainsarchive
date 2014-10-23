#ifndef _QTMAINWINDOW_H_
#define _QTMAINWINDOW_H_
#include <QtGui>
#include <QMap>
#include "SNODefinitions.h"
#include "QtAppearanceView.h"
#include "QtSettingsWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	signals:
		void actionUpdateCurrentAppearance(AppearanceView *pAppearance);

	public slots:
		void actionTabCurrentChanged(int nIndex);
		void actionTabCloseRequested(int nIndex);
		void actionOpenAppearance(SNOFiles::SNOFile *pSNOFile);
		void actionOpenTexture(SNOFiles::SNOFile *pSNOFile);
		void actionViewAppearanceOptions(void);
		void actionViewAssetBrowser(void);
		//void actionViewSettings(void);
		//void actionTest(void);

	private: //Controls
		void initializeControls(void);

		QTabWidget *m_pTabWidget;
		//QInputDialog *m_pInputDialog;

	private: //Dock windows
		void initializeDockWindows(void);

		QDockWidget *m_pAppearanceOptionsDock;
		QDockWidget *m_pAssetExplorerDock;

	private: //Actions
		void initializeActions(void);

		QAction *m_pFileExit;
		QAction *m_pViewAppearanceOptions;
		QAction *m_pViewAssetBrowser;
		//QAction *m_pViewSettings;
		//QAction *m_pTest;
		//QAction *m_pSave;
		//QAction *m_pSaveAll;

	//private: //Tool bars
	//	void initializeToolBars(void);

	//	QToolBar *m_pToolBarNewFiles;
	//	QToolBar *m_pToolBarSaving;

	private: //Menus
		void initializeMenu(void);

		QMenu *m_pViewMenu;

	//private:
	//	SettingsWindow *m_pSettingsWindow;

	private:
		HINSTANCE m_hInstance;

	public:
		MainWindow(HINSTANCE hInstance);
		~MainWindow(void);

		bool initialize(void);
		void shutdown(void);
};

#endif //#ifndef _QTMAINWINDOW_H_
