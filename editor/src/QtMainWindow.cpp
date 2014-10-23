#include "QtMainWindow.h"
#include "globals.h"
#include "settings.h"
#include "logger.h"
#include "resource.h"
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <StormLib.h>
#include "QtAssetBrowser.h"

#include "QtAppearanceOptions.h"
#include "QtTextureWidget.h"
#include "SNOManager.h"

MainWindow::MainWindow(HINSTANCE hInstance)
{
	m_pTabWidget = 0;
	//m_pSettingsWindow = 0;
}

bool MainWindow::initialize(void)
{
	SNOManager *pSNOManager = SNOManager::instance();

	QString szFileName = Settings::instance()->value("Application").toString();
	QString szFileLocale = Settings::instance()->value("FileLocale").toString();
	QString szPathName = szFileLocale;
	szPathName += "\\";
	szPathName += szFileName;

	QDir tempDir;

TestLocale:
	if(tempDir.exists(szPathName))
	{
		Settings::instance()->setValue("FileLocale", szFileLocale);

		szPathName = szFileLocale;
		szPathName += "\\Data_D3\\PC\\MPQs";
		pSNOManager->initialize(szPathName);
	}
	else
	{
		szFileLocale = QFileDialog::getExistingDirectory(this, tr("Locate your Diablo III install location."), QDir::currentPath());

		if(szFileLocale == "")
		{
			return false;
		}

		szPathName = szFileLocale;
		szPathName += "\\";
		szPathName += szFileName;

		goto TestLocale;
	}

	//TODO: debug code
	//pSNOManager->logTextureFormats();

	initializeActions();
	initializeControls();
	//initializeToolBars();
	initializeMenu();
	initializeDockWindows();

	//m_pSettingsWindow = new SettingsWindow(this);

	//Final main window initialize
	setWindowIcon(Globals::instance()->iconApplication());
	setWindowTitle(tr("Cain's Aide"));
	resize(1280, 1024);
	//setWindowState(Qt::WindowMaximized);
	statusBar()->showMessage(tr("").sprintf("%d Assets Mapped." , pSNOManager->mappedAssetCount()));
	show();

	return true;
}

void MainWindow::initializeControls(void)
{
	//Initialize Input Dialog
	//m_pInputDialog = new QInputDialog;
	//m_pInputDialog->setInputMode(QInputDialog::TextInput);
	//m_pInputDialog->setWindowIcon(Globals::instance()->iconSwatch());
	//m_pInputDialog->setWindowTitle(tr("New Tile Set"));
	//m_pInputDialog->setLabelText(tr("New Tile Set Name:"));
	//connect(m_pInputDialog, SIGNAL(textValueSelected(const QString &)), this, SLOT(actionNewTileSetNameSelected(const QString &)));

	m_pTabWidget = new QTabWidget;
	m_pTabWidget->setMovable(true);
	m_pTabWidget->setTabsClosable(true);
	connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(actionTabCurrentChanged(int)));
	connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(actionTabCloseRequested(int)));

	setCentralWidget(m_pTabWidget);
}

void MainWindow::initializeDockWindows(void)
{
	m_pAppearanceOptionsDock = new QDockWidget(tr("Appearance Options"), this);
	AppearanceOptions *pNewAppearanceOptions = new AppearanceOptions;
	pNewAppearanceOptions->initialize();
	connect(this, SIGNAL(actionUpdateCurrentAppearance(AppearanceView *)), pNewAppearanceOptions, SLOT(actionSetCurrentAppearance(AppearanceView *)));
	m_pAppearanceOptionsDock->setWidget(pNewAppearanceOptions);
	addDockWidget(Qt::LeftDockWidgetArea, m_pAppearanceOptionsDock);

	m_pAssetExplorerDock = new QDockWidget(tr("Asset Browser"), this);
	AssetBrowser *pNewAssetBrowser = new AssetBrowser(this);
	connect(pNewAssetBrowser, SIGNAL(openAppearance(SNOFiles::SNOFile *)), this,SLOT(actionOpenAppearance(SNOFiles::SNOFile *)));
	connect(pNewAssetBrowser, SIGNAL(openTexture(SNOFiles::SNOFile *)), this,SLOT(actionOpenTexture(SNOFiles::SNOFile *)));
	m_pAssetExplorerDock->setWidget(pNewAssetBrowser);
	addDockWidget(Qt::RightDockWidgetArea, m_pAssetExplorerDock);
}

void MainWindow::initializeActions(void)
{
	Globals *pGlobals = Globals::instance();

	m_pFileExit = new QAction(tr("E&xit"), this);
	m_pFileExit->setStatusTip(tr("Exit the application."));
	m_pFileExit->setIcon(Globals::instance()->iconCancel());
	connect(m_pFileExit, SIGNAL(triggered()), this, SLOT(close()));

	m_pViewAppearanceOptions = new QAction(tr("&Appearance Options"), this);
	m_pViewAppearanceOptions->setStatusTip(tr("Displays the Appearance Options dock window."));
	connect(m_pViewAppearanceOptions, SIGNAL(triggered()), this, SLOT(actionViewAppearanceOptions()));

	m_pViewAssetBrowser = new QAction(tr("Asset &Browser"), this);
	m_pViewAssetBrowser->setStatusTip(tr("Displays the Asset Browser dock window."));
	connect(m_pViewAssetBrowser, SIGNAL(triggered()), this, SLOT(actionViewAssetBrowser()));

	//m_pViewSettings = new QAction(tr("&Settings"), this);
	//m_pViewSettings->setStatusTip(tr("Displays the settings window."));
	//m_pViewSettings->setIcon(Globals::instance()->iconOptions());
	//connect(m_pViewSettings, SIGNAL(triggered()), this, SLOT(actionViewSettings()));

	//m_pTest = new QAction(tr("Test"), this);
	//m_pTest->setStatusTip(tr("Test"));
	//connect(m_pTest, SIGNAL(triggered()), this, SLOT(actionTest()));

	//m_pSave = new QAction(pGlobals->iconDisk(), tr("Save"), this);
	//m_pSave->setStatusTip(tr("Save current Map."));

	//m_pSaveAll = new QAction(pGlobals->iconDiskMulti(), tr("Save All"), this);
	//m_pSaveAll->setStatusTip(tr("Save all open Maps."));
}

//void MainWindow::initializeToolBars(void)
//{
//	//New Files Tool Bar
//	m_pToolBarNewFiles = addToolBar(tr("New Files Tool Bar"));
//	m_pToolBarNewFiles->addAction(m_pNewMap);
//	m_pToolBarNewFiles->addAction(m_pNewTileSet);
//
//	//Saving Tool bar
//	m_pToolBarSaving = addToolBar(tr("Saving Tool Bar"));
//	m_pToolBarSaving->addAction(m_pSave);
//	m_pToolBarSaving->addAction(m_pSaveAll);
//}

void MainWindow::initializeMenu(void)
{
	//File Menu
	m_pViewMenu = menuBar()->addMenu(tr("&View"));

	m_pViewMenu->addAction(m_pViewAppearanceOptions);
	m_pViewMenu->addAction(m_pViewAssetBrowser);
	//m_pViewMenu->addSeparator();
	//m_pViewMenu->addAction(m_pViewSettings);
	//m_pViewMenu->addAction(m_pTest);
}

void MainWindow::actionOpenAppearance(SNOFiles::SNOFile *pSNOFile)
{
	AppearanceView *pNewAppearance = new AppearanceView(m_pTabWidget);
	pNewAppearance->makeCurrent();
	m_pTabWidget->setCurrentIndex(m_pTabWidget->addTab(pNewAppearance, pSNOFile->SNOName));
	pNewAppearance->open(pSNOFile->uSNOIndex);
	emit actionUpdateCurrentAppearance(pNewAppearance);
}

void MainWindow::actionOpenTexture(SNOFiles::SNOFile *pSNOFile)
{
	TextureWidget *pNewTextureWidget = new TextureWidget;
	pNewTextureWidget->initialize();

	SNOTexture *pNewTextureFile = new SNOTexture;

	if(pNewTextureFile->open(pSNOFile->uSNOIndex))
	{
		pNewTextureWidget->open(pNewTextureFile);
		m_pTabWidget->setCurrentIndex(m_pTabWidget->addTab(pNewTextureWidget, pSNOFile->SNOName));
	}
}

void MainWindow::actionTabCurrentChanged(int nIndex)
{
	QWidget *pCurrentWidget = 0;
	pCurrentWidget = m_pTabWidget->widget(nIndex);

	if(pCurrentWidget)
	{
		if(pCurrentWidget->objectName() == tr("Appearance"))
		{
			((QGLWidget *)pCurrentWidget)->makeCurrent();

			emit actionUpdateCurrentAppearance((AppearanceView *)pCurrentWidget);
		}
	}
}

void MainWindow::actionTabCloseRequested(int nIndex)
{
	QWidget *pCurrentWidget = 0;
	
	//TODO: find a way to make this not set current tab to 0
	if((m_pTabWidget->count() - 1) > 0)
	{
		pCurrentWidget = m_pTabWidget->widget(0);
		if(pCurrentWidget)
		{
			if(pCurrentWidget->objectName() == tr("Appearance"))
			{
				((AppearanceView *)pCurrentWidget)->makeCurrent();

				emit actionUpdateCurrentAppearance((AppearanceView *)m_pTabWidget->widget(0));
			}
		}
		m_pTabWidget->setCurrentIndex(0);
	}
	else
	{
		emit actionUpdateCurrentAppearance(0);
	}

	pCurrentWidget = m_pTabWidget->widget(nIndex);
	m_pTabWidget->removeTab(nIndex);
	if(pCurrentWidget)
	{
		if(pCurrentWidget->objectName() == tr("Appearance"))
		{
			((AppearanceView *)pCurrentWidget)->makeCurrent();
		}

		delete pCurrentWidget;
		pCurrentWidget = 0;
	}
}

void MainWindow::actionViewAppearanceOptions(void)
{
	if(m_pAppearanceOptionsDock)
	{
		m_pAppearanceOptionsDock->show();
	}
}

void MainWindow::actionViewAssetBrowser(void)
{
	if(m_pAssetExplorerDock)
	{
		m_pAssetExplorerDock->show();
	}
}

//void MainWindow::actionViewSettings(void)
//{
//	m_pSettingsWindow->show();
//}

//void MainWindow::actionTest(void)
//{
//
//}

void MainWindow::shutdown(void)
{
	//if(m_pInputDialog)
	//{
	//	delete m_pInputDialog;
	//	m_pInputDialog = 0;
	//}
	if(m_pTabWidget)
	{
		while(m_pTabWidget->count())
		{
			QWidget *pCurrentWidget = 0;
			pCurrentWidget = m_pTabWidget->currentWidget();

			if(pCurrentWidget)
			{
				delete pCurrentWidget;
				pCurrentWidget = 0;
			}
		}

		delete m_pTabWidget;
		m_pTabWidget = 0;
	}

	//if(m_pSettingsWindow)
	//{
	//	delete m_pSettingsWindow;
	//	m_pSettingsWindow = 0;
	//}
}

MainWindow::~MainWindow(void)
{
	shutdown();
}
