#ifndef _QTASSETLISTITEM_H_
#define _QTASSETLISTITEM_H_
#include <QListWidgetItem>
#include "SNODefinitions.h"

class AssetListItem : public QListWidgetItem
{
	private:
		SNOFiles::SNOFile *m_pSNOFile;

	public:
		AssetListItem(void){m_pSNOFile = 0;}
		~AssetListItem(void){}

		void setSNOFile(SNOFiles::SNOFile *pSNOFile){m_pSNOFile = pSNOFile;}
		SNOFiles::SNOFile *snoFile(void){return m_pSNOFile;}
};

#endif //#ifndef _QTASSETLISTITEM_H_
