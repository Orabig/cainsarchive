#include "QtAppearanceOptions.h"
#include "Globals.h"

AppearanceOptions::AppearanceOptions(void)
{
	m_bInitialized = false;
	m_pCurrentAppearance = 0;
	m_uCurrentMaterialCount = 0;
	m_pCurrentDrawListItem = 0;
	m_bAlpha = true;
}

bool AppearanceOptions::initialize(void)
{
	if(m_bInitialized)
		shutdown();

	initializeControls();

	m_bInitialized = true;
	return m_bInitialized;
}

void AppearanceOptions::initializeControls(void)
{
	m_pLayout = new QVBoxLayout;
	m_pLayout->setContentsMargins(1, 1, 1, 1);
	m_pLayout->setAlignment(Qt::AlignTop);

	//Object and Material selection
	m_pObjectAndMaterialSelectionLayout = new QHBoxLayout;
	m_pObjectAndMaterialSelectionDialog = new QDialog;
	m_pObjectListComboBox = new QComboBox(m_pObjectAndMaterialSelectionDialog);
	connect(m_pObjectListComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(currentObjectIndexChanged(const QString &)));
	m_pObjectAndMaterialSelectionLayout->addWidget(m_pObjectListComboBox);

	m_pMaterialSelectionSpinBox = new QSpinBox(m_pObjectAndMaterialSelectionDialog);
	m_pObjectAndMaterialSelectionLayout->addWidget(m_pMaterialSelectionSpinBox);

	m_pObjectAndMaterialSelectionLayout->setStretch(0, 20);

	m_pObjectAndMaterialSelectionDialog->setLayout(m_pObjectAndMaterialSelectionLayout);
	m_pLayout->addWidget(m_pObjectAndMaterialSelectionDialog);

	//Draw list object + / - buttons
	m_pDrawListModifyLayout = new QHBoxLayout;
	m_pDrawListModifyDialog = new QDialog;

	m_pAddButton = new QPushButton(m_pDrawListModifyDialog);
	m_pAddButton->setIcon(Globals::instance()->iconAdd());
	connect(m_pAddButton, SIGNAL(clicked(void)), this, SLOT(actionAddButtonClicked(void)));
	m_pDrawListModifyLayout->addWidget(m_pAddButton);

	m_pMinusButton = new QPushButton(m_pDrawListModifyDialog);
	m_pMinusButton->setIcon(Globals::instance()->iconMinus());
	connect(m_pMinusButton, SIGNAL(clicked(void)), this, SLOT(actionMinusButtonClicked(void)));
	m_pDrawListModifyLayout->addWidget(m_pMinusButton);

	m_pDrawListModifyDialog->setLayout(m_pDrawListModifyLayout);
	m_pLayout->addWidget(m_pDrawListModifyDialog);

	//Draw list
	m_pDrawList = new QListWidget;
	connect(m_pDrawList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(actionDrawListItemDoubleClicked(QListWidgetItem *)));
	m_pLayout->addWidget(m_pDrawList);

	m_pDrawListMaterialInput = new QInputDialog;
	m_pDrawListMaterialInput->setWindowIcon(Globals::instance()->iconApplication());
	m_pDrawListMaterialInput->setWindowTitle(tr("Material"));
	m_pDrawListMaterialInput->setOption(QInputDialog::NoButtons);
	connect(m_pDrawListMaterialInput, SIGNAL(intValueChanged(int)), this, SLOT(actionDrawListMaterialInputIntValueChanged(int)));

	//Alpha
	m_pButtonGroup00Layout = new QHBoxLayout;
	m_pButtonGroup00Dialog = new QDialog;

	m_pAlphaButton = new QPushButton(m_pButtonGroup00Dialog);
	m_pAlphaButton->setText(tr("Alpha Enabled"));
	m_pAlphaButton->setFont(QFont("Arial", 10, QFont::Bold));
	m_AlphaButtonPalette = m_pAlphaButton->palette();
	m_AlphaButtonPalette.setColor(QPalette::ButtonText, QColor(0, 192, 0, 255));
	m_pAlphaButton->setPalette(m_AlphaButtonPalette);
	connect(m_pAlphaButton, SIGNAL(clicked(void)), this, SLOT(actionAlphaClicked(void)));
	m_pButtonGroup00Layout->addWidget(m_pAlphaButton);

	m_pBackgroundColorButton = new QPushButton(m_pButtonGroup00Dialog);
	m_pBackgroundColorButton->setText(tr("Background Color"));
	m_pBackgroundColorButton->setFont(QFont("Arial", 10, QFont::Bold));
	m_AlphaButtonPalette = m_pBackgroundColorButton->palette();
	m_AlphaButtonPalette.setColor(QPalette::ButtonText, QColor(0, 0, 0, 255));
	m_pBackgroundColorButton->setPalette(m_AlphaButtonPalette);
	connect(m_pBackgroundColorButton, SIGNAL(clicked(void)), this, SLOT(actionBackgroundColorClicked(void)));
	m_pButtonGroup00Layout->addWidget(m_pBackgroundColorButton);

	connect(&m_ColorPicker, SIGNAL(colorSelected(const QColor &)), this, SLOT(actionColorSelected(const QColor &)));

	m_pButtonGroup00Dialog->setLayout(m_pButtonGroup00Layout);
	m_pLayout->addWidget(m_pButtonGroup00Dialog);

	setLayout(m_pLayout);

	m_pObjectListComboBox->setDisabled(true);
	m_pMaterialSelectionSpinBox->setDisabled(true);
	m_pAddButton->setDisabled(true);
	m_pMinusButton->setDisabled(true);
	m_pDrawList->setDisabled(true);
	m_pAlphaButton->setDisabled(true);
	m_pBackgroundColorButton->setDisabled(true);
}

void AppearanceOptions::currentObjectIndexChanged(const QString &szText)
{
	m_pMaterialSelectionSpinBox->setValue(0);
	m_pMaterialSelectionSpinBox->setMinimum(0);
	m_pMaterialSelectionSpinBox->setMaximum(0);

	if(!szText.isNull())
	{
		m_pMaterialSelectionSpinBox->setMaximum(m_pCurrentAppearance->objectMaterialCount(szText) - 1);
	}
}

void AppearanceOptions::actionSetCurrentAppearance(AppearanceView *pAppearance)
{
	if(pAppearance == 0)
	{
		m_pObjectListComboBox->clear();
		m_pObjectListComboBox->setDisabled(true);

		m_pMaterialSelectionSpinBox->setValue(0);
		m_pMaterialSelectionSpinBox->setDisabled(true);

		for(int i = 0; i < m_pDrawList->count(); i++)
		{
			if(m_pDrawList->item(i))
			{
				delete m_pDrawList->item(i);
			}
		}
		m_pDrawList->clear();
		m_pDrawList->setDisabled(true);

		m_pAddButton->setDisabled(true);
		m_pMinusButton->setDisabled(true);

		m_pAlphaButton->setDisabled(true);
		m_pBackgroundColorButton->setDisabled(true);

		return;
	}

	m_pCurrentAppearance = pAppearance;

	updateObjectList(m_pCurrentAppearance->objectList());

	m_bAlpha = !m_pCurrentAppearance->alphaDiscard();
	actionAlphaClicked();

	m_pObjectListComboBox->setDisabled(false);
	m_pMaterialSelectionSpinBox->setDisabled(false);
	m_pAddButton->setDisabled(false);
	m_pMinusButton->setDisabled(false);
	m_pDrawList->setDisabled(false);
	m_pAlphaButton->setDisabled(false);
	m_pBackgroundColorButton->setDisabled(false);
}

void AppearanceOptions::actionAlphaClicked(void)
{
	if(m_pCurrentAppearance)
	{
		if(m_bAlpha)
		{
			m_bAlpha = false;

			m_pAlphaButton->setText(tr("Alpha Disabled"));
			m_AlphaButtonPalette.setColor(QPalette::ButtonText, QColor(192, 0, 0, 255));
			m_pAlphaButton->setPalette(m_AlphaButtonPalette);
		}
		else
		{
			m_bAlpha = true;

			m_pAlphaButton->setText(tr("Alpha Enabled"));
			m_AlphaButtonPalette.setColor(QPalette::ButtonText, QColor(0, 192, 0, 255));
			m_pAlphaButton->setPalette(m_AlphaButtonPalette);
		}

		m_pCurrentAppearance->setAlphaDiscard(m_bAlpha);
	}
}

void AppearanceOptions::actionBackgroundColorClicked(void)
{
	if(m_pCurrentAppearance)
	{
		m_ColorPicker.setCurrentColor
		(
			QColor
			(
				(int)(m_pCurrentAppearance->clearColor().r * 255),
				(int)(m_pCurrentAppearance->clearColor().g * 255),
				(int)(m_pCurrentAppearance->clearColor().b * 255),
				255
			)
		);
		m_ColorPicker.exec();
	}
}

void AppearanceOptions::actionColorSelected(const QColor &newColor)
{
	if(m_pCurrentAppearance)
	{
		fRGBAColor tempColor;
		tempColor.r = newColor.red() / 255.0f;
		tempColor.g = newColor.green() / 255.0f;
		tempColor.b = newColor.blue() / 255.0f;
		tempColor.a = 0.0f;

		m_pCurrentAppearance->setClearColor(tempColor);
	}
}

void AppearanceOptions::updateObjectList(QVector<QString> *pVObjects)
{
	if(pVObjects == 0)
		return;

	m_pObjectListComboBox->clear();

	for(int i = 0; i < pVObjects->size(); i++)
	{
		m_pObjectListComboBox->addItem((*pVObjects)[i]);
	}

	if(m_pCurrentAppearance->drawList().size() <= 0)
	{
		if(m_pCurrentAppearance->drawList().size() == 1)
		{
			AppearanceDrawItem *pNewItem = new AppearanceDrawItem;
			pNewItem->szObjectName = (*pVObjects)[0];
			pNewItem->uObjectMaterial = 0;
			m_pCurrentAppearance->addItemToDrawList(pNewItem);
			goto EndUpdateObjectList;
		}

		//Find oneBatch
		for(int i = 0; i < pVObjects->size(); i++)
		{
			if((*pVObjects)[i].contains(tr("onebatch"), Qt::CaseInsensitive))
			{
				AppearanceDrawItem *pNewItem = new AppearanceDrawItem;
				pNewItem->szObjectName = (*pVObjects)[i];
				pNewItem->uObjectMaterial = 0;
				m_pCurrentAppearance->addItemToDrawList(pNewItem);
				goto EndUpdateObjectList;
			}
		}

		//Find normal_mat
		for(int i = 0; i < pVObjects->size(); i++)
		{
			if((*pVObjects)[i].contains(tr("normal_mat"), Qt::CaseInsensitive))
			{
				AppearanceDrawItem *pNewItem = new AppearanceDrawItem;
				pNewItem->szObjectName = (*pVObjects)[i];
				pNewItem->uObjectMaterial = 0;
				m_pCurrentAppearance->addItemToDrawList(pNewItem);
				goto EndUpdateObjectList;
			}
		}

		//Find any mat
		for(int i = 0; i < pVObjects->size(); i++)
		{
			if((*pVObjects)[i].contains(tr("mat"), Qt::CaseInsensitive))
			{
				AppearanceDrawItem *pNewItem = new AppearanceDrawItem;
				pNewItem->szObjectName = (*pVObjects)[i];
				pNewItem->uObjectMaterial = 0;
				m_pCurrentAppearance->addItemToDrawList(pNewItem);
				goto EndUpdateObjectList;
			}
		}

		AppearanceDrawItem *pNewItem = new AppearanceDrawItem;
		pNewItem->szObjectName = (*pVObjects)[pVObjects->size() - 1];
		pNewItem->uObjectMaterial = 0;
		m_pCurrentAppearance->addItemToDrawList(pNewItem);

		EndUpdateObjectList:
		syncAppearanceDrawList();
	}
	else
	{
		syncAppearanceDrawList();
	}

	delete pVObjects;
}

void AppearanceOptions::syncAppearanceDrawList(void)
{
	for(int i = 0; i < m_pDrawList->count(); i++)
	{
		QListWidgetItem *pCurrentItem = 0;
		pCurrentItem = m_pDrawList->item(i);
		if(pCurrentItem)
		{
			delete pCurrentItem;
			pCurrentItem = 0;
		}
	}
	m_pDrawList->clear();

	for(int i = 0; i < m_pCurrentAppearance->drawList().size(); i++)
	{
		AppearanceDrawItem *pCurrentItem = 0;
		pCurrentItem = m_pCurrentAppearance->drawList()[i];

		if(pCurrentItem)
		{
			DrawListItem *pNewItem = new DrawListItem;
			pNewItem->setObjectName(pCurrentItem->szObjectName);
			pNewItem->setObjectMaterial(pCurrentItem->uObjectMaterial);

			QString szTemp;
			szTemp = pCurrentItem->szObjectName + szTemp.sprintf("[%d]", pCurrentItem->uObjectMaterial);
			pNewItem->setText(szTemp);

			m_pDrawList->addItem(pNewItem);
		}
	}
}

void AppearanceOptions::actionAddButtonClicked(void)
{
	DrawListItem *pNewItem = new DrawListItem;
	QString szNewListText = m_pObjectListComboBox->currentText();

	pNewItem->setObjectName(szNewListText);
	pNewItem->setObjectMaterial(m_pMaterialSelectionSpinBox->value());

	szNewListText = m_pObjectListComboBox->currentText() + szNewListText.sprintf("[%d]", m_pMaterialSelectionSpinBox->value());
	pNewItem->setText(szNewListText);

	if(m_pDrawList->findItems(szNewListText, Qt::MatchExactly).count() > 0)
	{
		delete pNewItem;
		return;
	}

	m_pDrawList->addItem(pNewItem);
	
	AppearanceDrawItem *pNewDrawItem = new AppearanceDrawItem;
	pNewDrawItem->szObjectName = pNewItem->objectName();
	pNewDrawItem->uObjectMaterial = pNewItem->objectMaterial();

	m_pCurrentAppearance->addItemToDrawList(pNewDrawItem);
}

void AppearanceOptions::actionMinusButtonClicked(void)
{
	QList<QListWidgetItem *> itemList = m_pDrawList->selectedItems();

	if(itemList.count())
	{
		for(int i = 0; i < itemList.count(); i++)
		{
			AppearanceDrawItem *pNewDrawItem = new AppearanceDrawItem;
			pNewDrawItem->szObjectName = ((DrawListItem *)itemList[i])->objectName();
			pNewDrawItem->uObjectMaterial = ((DrawListItem *)itemList[i])->objectMaterial();
			m_pCurrentAppearance->removeItemFromDrawList(pNewDrawItem);
			delete pNewDrawItem;

			m_pDrawList->removeItemWidget(itemList[i]);
			delete itemList[i];
		}
	}
}

void AppearanceOptions::actionDrawListItemDoubleClicked(QListWidgetItem *pItem)
{
	if(pItem && m_bInitialized)
	{
		m_pCurrentDrawListItem = (DrawListItem *)pItem;
		m_pDrawListMaterialInput->setInputMode(QInputDialog::IntInput);
		m_pDrawListMaterialInput->setLabelText(m_pCurrentDrawListItem->objectName() + tr(" Material: "));
		m_pDrawListMaterialInput->setIntMinimum(0);
		m_pDrawListMaterialInput->setIntValue(m_pCurrentDrawListItem->objectMaterial());
		m_pDrawListMaterialInput->setIntMaximum(m_pCurrentAppearance->objectMaterialCount(m_pCurrentDrawListItem->objectName()) - 1);
		m_pDrawListMaterialInput->exec();
	}
}

void 	AppearanceOptions::actionDrawListMaterialInputIntValueChanged(int nValue)
{
	if(m_pCurrentDrawListItem)
	{
		AppearanceDrawItem *pNewDrawitem = new AppearanceDrawItem;
		pNewDrawitem->szObjectName = m_pCurrentDrawListItem->objectName();
		pNewDrawitem->uObjectMaterial = m_pCurrentDrawListItem->objectMaterial();

		m_pCurrentAppearance->removeItemFromDrawList(pNewDrawitem);

		m_pCurrentDrawListItem->setObjectMaterial((uint32)nValue);
		m_pCurrentDrawListItem->setText(m_pCurrentDrawListItem->objectName() + tr("").sprintf("[%d]", nValue));
		
		pNewDrawitem->uObjectMaterial = (uint32)nValue;

		m_pCurrentAppearance->addItemToDrawList(pNewDrawitem);
	}
}

QSize AppearanceOptions::sizeHint(void) const
{
	return QSize(250, 100);
}

void AppearanceOptions::shutdown(void)
{
	if(!m_bInitialized)
		return;

	m_bInitialized = false;
}

AppearanceOptions::~AppearanceOptions(void)
{
	shutdown();
}
