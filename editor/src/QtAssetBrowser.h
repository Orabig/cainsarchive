#ifndef _QTMPQEXPLORER_H_
#define _QTMPQEXPLORER_H_
#include <QtGui>
#include "SNODefinitions.h"

class AssetBrowser : public QMainWindow
{
	Q_OBJECT

	signals:
		void openAppearance(SNOFiles::SNOFile *pSNOFile);
		void openTexture(SNOFiles::SNOFile *pSNOFile);

	public slots:
		void actionFilter(const QString &text);
		void actionDoubleClicked(QListWidgetItem *pItem);

	private: //Controls
		void initializeControls(void);

		QTabWidget *m_pTabWidget;

		QDialog *m_pApperanceListDialog;
		QVBoxLayout *m_pApperanceTabLayout;
		QLineEdit *m_pApperanceFilterEdit;
		QListWidget *m_pApperanceList;

		QDialog *m_pTextureListDialog;
		QVBoxLayout *m_pTextureTabLayout;
		QLineEdit *m_pTextureFilterEdit;
		QListWidget *m_pTextureList;

	private:
		void filterList(QListWidget *pList, const QString &filterText);

	public:
		AssetBrowser(QWidget *parent = 0);

		QSize sizeHint(void) const;
};


#endif //#ifndef _QTMPQEXPLORER_H_
