#ifndef _SNOMANAGER_H_
#define _SNOMANAGER_H_
#include <glew.h>
#include <wglew.h>
#include <QString>
#include <QMap>
#include <StormLib.h>
#include "SNODefinitions.h"
#include "Types.h"

class SNOManager
{
	private:
		bool m_bInitialized;

		QMap<QString, HANDLE> m_mArchives; 
		QMap<uint32, SNOFiles::SNOFile *> m_mFileIndex;

		QString m_szCoreDataArchiveName;
		QString m_szClientDataArchiveName;
		QString m_szTextureArchiveName;

		bool linkPatches(const QString &archiveName, HANDLE hArcive, const QString &clientMPQPath);
		QString makePathName(const uint32 &uSNOType, const QString &fileName);

	private:
		static SNOManager *m_pInstance;

		SNOManager(void);
		SNOManager(const SNOManager &ref){}
		SNOManager &operator=(const SNOManager &ref){}

	public:
		~SNOManager(void);

		static SNOManager *instance(void);
		static void deleteInstance(void);

		bool initialize(const QString &MPQPath);
		void shutdown(void);

		void logTextureFormats(void);

		QVector<SNOFiles::SNOFile *> *appearances(void);
		QVector<SNOFiles::SNOFile *> *textures(void);

		HANDLE openFile(const uint32 &uSNOIndex);

		uint32 mappedAssetCount(void){return (uint32)m_mFileIndex.size();}
};

#endif //#ifndef _SNOMANAGER_H_
