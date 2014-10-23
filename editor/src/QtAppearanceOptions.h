#ifndef _APPEARANCEOPTIONS_H_
#define _APPEARANCEOPTIONS_H_
#include <QDialog>
#include <QString>
#include <QVector>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QInputDialog>
#include <QColorDialog>
#include "Types.h"
#include "QtAppearanceView.h"
#include "AppearanceOptionsDrawListItem.h"

class AppearanceOptions : public QDialog
{
	Q_OBJECT

	public slots:
		void actionSetCurrentAppearance(AppearanceView *pAppearance);
		void currentObjectIndexChanged(const QString &szText);
		void actionAddButtonClicked(void);
		void actionMinusButtonClicked(void);
		void actionDrawListItemDoubleClicked(QListWidgetItem *pItem);
		void 	actionDrawListMaterialInputIntValueChanged(int nValue);
		void actionAlphaClicked(void);
		void actionBackgroundColorClicked(void);
		void actionColorSelected(const QColor &newColor);
		//void 	actionDrawListMaterialInputIntValueSelected(int nValue);

	private: //Controls
		void initializeControls(void);

		QVBoxLayout *m_pLayout;

		//Object and Material selection
		QHBoxLayout *m_pObjectAndMaterialSelectionLayout;
		QDialog *m_pObjectAndMaterialSelectionDialog;
		QComboBox *m_pObjectListComboBox;
		QSpinBox *m_pMaterialSelectionSpinBox;

		//Draw list object + / - buttons
		QHBoxLayout *m_pDrawListModifyLayout;
		QDialog *m_pDrawListModifyDialog;
		QPushButton *m_pAddButton;
		QPushButton *m_pMinusButton;

		//Draw list
		QListWidget *m_pDrawList;

		QInputDialog *m_pDrawListMaterialInput;
		DrawListItem *m_pCurrentDrawListItem;

		//Button Group 00
		bool m_bAlpha;
		QHBoxLayout *m_pButtonGroup00Layout;
		QDialog *m_pButtonGroup00Dialog;
		QPushButton *m_pAlphaButton;
		QPalette m_AlphaButtonPalette;
		QPushButton *m_pBackgroundColorButton;
		QColorDialog m_ColorPicker;

	private:
		void updateObjectList(QVector<QString> *pVObjects);
		void syncAppearanceDrawList(void);

	private:
		bool m_bInitialized;

		AppearanceView *m_pCurrentAppearance;
		uint32 m_uCurrentMaterialCount;

	public:
		AppearanceOptions(void);
		~AppearanceOptions(void);

		bool initialize(void);
		void shutdown(void);

		QSize sizeHint(void) const;
};

#endif //#ifndef _APPEARANCEOPTIONS_H_
