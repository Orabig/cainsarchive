#ifndef _DCR_GLOBALS_H_
#define _DCR_GLOBALS_H_
#include <QString>
#include <QIcon>
#include "Types.h"

class Globals
{
	private:
		static Globals *m_pInstance;

		HINSTANCE m_hInstance;

		//Icons
		QIcon m_IconApplication;
		QIcon m_IconAdd;
		QIcon m_IconCancel;
		QIcon m_IconCompress;
		QIcon m_IconDisk;
		QIcon m_IconDiskMulti;
		QIcon m_IconFolder;
		QIcon m_IconMinus;
		QIcon m_IconOptions;

	private:
		Globals(void);
		Globals(const Globals &ref){}
		Globals &operator=(const Globals &ref){}

	public:
		~Globals(void);
		static Globals *instance(void);
		static void deleteInstance(void);

		bool initialize(HINSTANCE hInstance);
		void shutdown(void);

		//Hashing Functions (Diablo 3)
		uint32 StringHashD3(const QString &toHash, bool toLower = true);

		//Icons
		QIcon &iconApplication(void){return m_IconApplication;}
		QIcon &iconAdd(void){return m_IconAdd;}
		QIcon &iconCancel(void){return m_IconCancel;}
		QIcon &iconCompress(void){return m_IconCompress;}
		QIcon &iconDisk(void){return m_IconDisk;}
		QIcon &iconDiskMulti(void){return m_IconDiskMulti;}
		QIcon &iconFolder(void){return m_IconFolder;}
		QIcon &iconMinus(void){return m_IconMinus;}
		QIcon &iconOptions(void){return m_IconOptions;}

		QPixmap PixmapFromHICON(const HICON icon);
};

#endif //#ifndef _DCR_GLOBALS_H_
