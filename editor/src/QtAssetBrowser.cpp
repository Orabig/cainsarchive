#include "QtAssetBrowser.h"
#include "SNOManager.h"
#include "AssetListItem.h"

using namespace SNOFiles;

AssetBrowser::AssetBrowser(QWidget *parent)
{
	initializeControls();
}

QSize AssetBrowser::sizeHint(void) const
{
	return QSize(250, 100);
}

void AssetBrowser::initializeControls(void)
{
	m_pTabWidget = new QTabWidget;
	m_pTabWidget->setMovable(true);
	m_pTabWidget->setTabsClosable(false);
	m_pTabWidget->setContentsMargins(0, 0, 0, 0);

	//connect(m_pTabWidget, SIGNAL(currentChanged(int)), this, SLOT(actionCurrentTabChanged(int)));
	//connect(m_pTabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(actionCloseTabRequested(int)));

	m_pApperanceListDialog = new QDialog;
	m_pApperanceTabLayout = new QVBoxLayout(m_pApperanceListDialog);

	m_pApperanceFilterEdit = new QLineEdit(m_pApperanceListDialog);
	connect(m_pApperanceFilterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(actionFilter(const QString &)));

	m_pApperanceList = new QListWidget(m_pApperanceListDialog);
	connect(m_pApperanceList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(actionDoubleClicked(QListWidgetItem *)));

	m_pApperanceTabLayout->addWidget(m_pApperanceFilterEdit);
	m_pApperanceTabLayout->addWidget(m_pApperanceList);
	m_pApperanceListDialog->setLayout(m_pApperanceTabLayout);

	m_pTabWidget->addTab(m_pApperanceListDialog, tr("Models"));

	QVector<SNOFile *> *pActors = 0;
	pActors = SNOManager::instance()->appearances();

	if(pActors)
	{
		AssetListItem *pNewAssetListItem = 0;
		for(int i = 0; i < pActors->size(); i++)
		{
			pNewAssetListItem = new AssetListItem;

			if(pNewAssetListItem)
			{
				pNewAssetListItem->setSNOFile((*pActors)[i]);
				pNewAssetListItem->setText((*pActors)[i]->SNOName);
				m_pApperanceList->addItem(pNewAssetListItem);
			}
			pNewAssetListItem = 0;
		}

		delete pActors;
		pActors = 0;
	}

	m_pApperanceList->sortItems();

	m_pTextureListDialog = new QDialog;
	m_pTextureTabLayout = new QVBoxLayout(m_pTextureListDialog);

	m_pTextureFilterEdit = new QLineEdit(m_pTextureListDialog);
	connect(m_pTextureFilterEdit, SIGNAL(textChanged(const QString &)), this, SLOT(actionFilter(const QString &)));

	m_pTextureList = new QListWidget(m_pTextureListDialog);
	connect(m_pTextureList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(actionDoubleClicked(QListWidgetItem *)));

	m_pTextureTabLayout->addWidget(m_pTextureFilterEdit);
	m_pTextureTabLayout->addWidget(m_pTextureList);
	m_pTextureListDialog->setLayout(m_pApperanceTabLayout);

	m_pTabWidget->addTab(m_pTextureListDialog, tr("Textures"));

	QVector<SNOFile *> *pTexture = 0;
	pTexture = SNOManager::instance()->textures();

	if(pTexture)
	{
		AssetListItem *pNewAssetListItem = 0;
		for(int i = 0; i < pTexture->size(); i++)
		{
			pNewAssetListItem = new AssetListItem;

			if(pNewAssetListItem)
			{
				pNewAssetListItem->setSNOFile((*pTexture)[i]);
				pNewAssetListItem->setText((*pTexture)[i]->SNOName);
				m_pTextureList->addItem(pNewAssetListItem);
			}
			pNewAssetListItem = 0;
		}

		delete pTexture;
		pTexture = 0;
	}

	m_pTextureList->sortItems();

	setCentralWidget(m_pTabWidget);
}

void AssetBrowser::filterList(QListWidget *pList, const QString &filterText)
{
	for(int i = 0; i < pList->count(); i++)
	{
		if(filterText.isNull())
		{
			pList->item(i)->setHidden(false);
			continue;
		}

		if(!(pList->item(i)->text().contains(filterText, Qt::CaseInsensitive)))
		{
			pList->item(i)->setHidden(true);
		}
		else
		{
			pList->item(i)->setHidden(false);
		}
	}
}

void AssetBrowser::actionFilter(const QString &text)
{
	if(QObject::sender() == m_pApperanceFilterEdit)
	{
		filterList(m_pApperanceList, text);
		return;
	}

	if(QObject::sender() == m_pTextureFilterEdit)
	{
		filterList(m_pTextureList, text);
		return;
	}
}

void AssetBrowser::actionDoubleClicked(QListWidgetItem *pItem)
{
	if(pItem)
	{
		if(QObject::sender() == m_pApperanceList)
		{
			emit openAppearance(((AssetListItem *)pItem)->snoFile());
			return;
		}

		if(QObject::sender() == m_pTextureList)
		{
			emit openTexture(((AssetListItem *)pItem)->snoFile());
			return;
		}
	}
}
