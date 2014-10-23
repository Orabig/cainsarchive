#ifndef _APPEARANCEOPTIONSDRAWLISTITEM_H_
#define _APPEARANCEOPTIONSDRAWLISTITEM_H_
#include <QListWidgetItem>
#include <QString>

class DrawListItem : public QListWidgetItem
{
	private:
		QString m_szObjectName;
		uint32 m_uMaterial;

	public:
		DrawListItem(void){}
		~DrawListItem(void){}

		void setObjectName(const QString &szObjectName){m_szObjectName = szObjectName;}
		void setObjectMaterial(uint32 uMaterial){m_uMaterial = uMaterial;}

		const QString &objectName(void){return m_szObjectName;}
		uint32 objectMaterial(void){return m_uMaterial;}
};

#endif //#ifndef _APPEARANCEOPTIONSDRAWLISTITEM_H_
