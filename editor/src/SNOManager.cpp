#include "SNOManager.h"
#include <QDir>
#include <QVector>
#include "Globals.h"
#include "Logger.h"
#include <QMessageBox>

using namespace SNOFiles;

SNOManager *SNOManager::m_pInstance = 0;

SNOManager::SNOManager(void)
{
	m_bInitialized = false;

	m_szCoreDataArchiveName = "CoreData.mpq";
	m_szClientDataArchiveName = "ClientData.mpq";
	m_szTextureArchiveName = "Texture.mpq";
}

bool SNOManager::initialize(const QString &MPQPath)
{
	if(MPQPath.isNull())
		return false;

	if(m_bInitialized)
		shutdown();

	QVector<QString> vArchivePathNames;
	QVector<QDir> vDirectories;
	vDirectories.push_back(QDir(MPQPath));

	while(!vDirectories.isEmpty())
	{
		QDir currentDir = vDirectories.front();
		QString currentPath = currentDir.absolutePath();
		currentPath += "/";

		//currentDir.setFilter(QDir::Dirs);
		//QStringList slCurrentDirectoryEntries = currentDir.entryList();
		//for(int i = 0; i < slCurrentDirectoryEntries.size(); i++)
		//{
		//	if(slCurrentDirectoryEntries[i] == "." || slCurrentDirectoryEntries[i] == "..")
		//		continue;

		//	vDirectories.push_back(QDir(currentPath + slCurrentDirectoryEntries[i]));
		//}

		currentDir.setFilter(QDir::Files);
		QStringList slCurrentFileEntries = currentDir.entryList();
		for(int i = 0; i < slCurrentFileEntries.size(); i++)
		{
			QString fileName = slCurrentFileEntries[i];
			QString pathName = currentPath + fileName;
			HANDLE currentHandle = 0;
			if(SFileOpenArchive(pathName.toAscii().data(), 0, MPQ_OPEN_READ_ONLY, &currentHandle))
			{
				m_mArchives.insert(fileName, currentHandle);
				linkPatches(fileName, currentHandle, MPQPath);
			}

			//QString logString;
			//logString = slCurrentFileEntries[i];
			//Logger::instance()->logString(logString);
		}

		vDirectories.pop_front();
	}

	//Open/Load/Parse TOC file for game Assets
	HANDLE hTOC = 0;
	if(SFileOpenFileEx(m_mArchives[m_szCoreDataArchiveName], "CoreTOC.dat", SFILE_OPEN_PATCHED_FILE, &hTOC))
	{
		DWORD dwFileSize = 0;
		dwFileSize = SFileGetFileSize(hTOC);
		if(dwFileSize != SFILE_INVALID_SIZE)
		{
			char *pTOCBuffer = new char[dwFileSize];
			ZeroMemory(pTOCBuffer, dwFileSize);

			if(SFileReadFile(hTOC, pTOCBuffer, dwFileSize))
			{
				uint32 uCurrentPosition = 4; //Skip TOC SNO ID
				while(uCurrentPosition < dwFileSize)
				{
					//Read SNO Type
					uint32 uCurrentSNOType = 0;
					uCurrentSNOType = *((uint32 *)&pTOCBuffer[uCurrentPosition]);
					uCurrentPosition += sizeof(uint32);

					//Read SNO Index
					uint32 uCurrentSNOIndex = 0;
					uCurrentSNOIndex = *((uint32 *)&pTOCBuffer[uCurrentPosition]);
					uCurrentPosition += sizeof(uint32);

					QString SNOFileName;
					uint32 uNamePosition = 0;
					while(pTOCBuffer[uCurrentPosition + uNamePosition])
					{
						SNOFileName += pTOCBuffer[uCurrentPosition + uNamePosition];
						uNamePosition++;
					}

					SNOFile *pNewSNOFile = new SNOFile;
					pNewSNOFile->uSNOIndex = uCurrentSNOIndex;
					pNewSNOFile->uSNOType = uCurrentSNOType;
					pNewSNOFile->SNOName = SNOFileName;
					pNewSNOFile->hStormFile = 0;

					m_mFileIndex.insert(uCurrentSNOIndex, pNewSNOFile);
	
					//TODO: Debug Code, delete?
					//QString logString;
					//logString = logString.sprintf("RefLoaded: ID: %d Type: %d : %s", pNewSNOFile->uSNOIndex, pNewSNOFile->uSNOType, pNewSNOFile->SNOName.toAscii().data());
					//Logger::instance()->logString(logString);

					uCurrentPosition += 128; //TOC Name Block 128 bytes
				}

				QString logString;
				logString = logString.sprintf("Loaded %d Assets References", (uint32)m_mFileIndex.size());
				Logger::instance()->logString(logString);
			}
		}
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText("Cannot find Table of Contents!");
		msgBox.exec();
	}

	m_bInitialized = true;
	return m_bInitialized;
}

void SNOManager::logTextureFormats(void)
{
	if(!m_bInitialized)
		return;

	uint32 uFormats[512];
	ZeroMemory(uFormats, 512 * sizeof(uint32));

	QMapIterator<uint32, SNOFile *> iFiles(m_mFileIndex);
	while(iFiles.hasNext())
	{
		iFiles.next();
		if(iFiles.value()->uSNOType == SNOTypes::Surface || iFiles.value()->uSNOType == SNOTypes::Textures)
		{
			HANDLE hFile = 0;
			hFile = openFile(iFiles.value()->uSNOIndex);
			if(hFile)
			{
				DWORD dwFileSize = 0;
				dwFileSize = SFileGetFileSize(hFile);

				unsigned char *ucBuffer = new unsigned char[dwFileSize];
				ZeroMemory(ucBuffer, dwFileSize);
				SFileSetFilePointer(hFile, 0, 0, FILE_BEGIN);
				if(SFileReadFile(hFile, ucBuffer, dwFileSize))
				{
					TextureHeader *pTextureHeader = (TextureHeader *)&ucBuffer[sizeof(SNOHeader)];
					uFormats[pTextureHeader->uPixelFormat]++;
				}

				if(ucBuffer)
				{
					delete [] ucBuffer;
					ucBuffer = 0;
				}
			}
		}
	}

	for(uint32 i = 0; i < 512; i++)
	{
		QString logString;
		logString = logString.sprintf("Format %d = %d", i, uFormats[i]);
		Logger::instance()->logString(logString);
	}
}

QVector<SNOFile *> *SNOManager::appearances(void)
{
	if(!m_bInitialized)
		return 0;

	QVector<SNOFile *> *pNewVector = 0;
	pNewVector = new QVector<SNOFile *>;

	if(pNewVector)
	{
		QMapIterator<uint32, SNOFile *> iFiles(m_mFileIndex);

		while(iFiles.hasNext())
		{
			iFiles.next();
			if(iFiles.value()->uSNOType == SNOTypes::Appearance)
			{
				pNewVector->push_back(iFiles.value());
			}
		}
	}

	return pNewVector;
}

QVector<SNOFile *> *SNOManager::textures(void)
{
	if(!m_bInitialized)
		return 0;

	QVector<SNOFile *> *pNewVector = 0;
	pNewVector = new QVector<SNOFile *>;

	if(pNewVector)
	{
		QMapIterator<uint32, SNOFile *> iFiles(m_mFileIndex);

		while(iFiles.hasNext())
		{
			iFiles.next();
			if(iFiles.value()->uSNOType == SNOTypes::Surface || iFiles.value()->uSNOType == SNOTypes::Textures)
			{
				pNewVector->push_back(iFiles.value());
			}
		}
	}

	return pNewVector;
}

HANDLE SNOManager::openFile(const uint32 &uSNOIndex)
{
	if(!m_bInitialized)
		return 0;

	SNOFile *pFile = 0;
	pFile = m_mFileIndex[uSNOIndex];

	if(pFile)
	{
		if(pFile->hStormFile)
		{
			return pFile->hStormFile;
		}
	}
	else
	{
		return 0;
	}

	QString filePathName(makePathName(pFile->uSNOType, pFile->SNOName));

	HANDLE hArchive = 0;

	switch(pFile->uSNOType)
	{
		case SNOTypes::Actor:
		case SNOTypes::Appearance:
		{
			hArchive = m_mArchives[m_szClientDataArchiveName];
		}
		break;

		case SNOTypes::Surface:
		case SNOTypes::Textures:
		{
			hArchive = m_mArchives[m_szTextureArchiveName];
		}
		break;

		default:
			break;
	}

	if(hArchive <= 0)
		return 0;

	//TODO: We can't do this.. we should be able to do this however
	//Stormlib does not reference files which did not exist in the base MPQ
	//This means SFileHasFile will fail if the file was added in a linked patch file
	//this should be easily fixed by adding the functionality of SFileOpenFileEx
	//which scans all linked patches for the file as well as the base MPQ
	//if(!SFileHasFile(hArchive, texturePathName.toAscii().data()))
	//	return 0;

	HANDLE hFile = 0;
	if(!SFileOpenFileEx(hArchive, filePathName.toAscii().data(), SFILE_OPEN_PATCHED_FILE, &hFile))
		return 0;

	if(pFile)
	{
		pFile->hStormFile = hFile;
	}

	return hFile;
}

QString SNOManager::makePathName(const uint32 &uSNOType, const QString &fileName)
{
	QString filePathName;
	QString fileExtension;

	//TODO: Update as needed for file paths and types.
	switch(uSNOType)
	{
		case SNOTypes::Actor:
		case SNOTypes::Appearance:
		{
			filePathName = "Appearance";
			fileExtension = ".app";
		}
		break;

		case SNOTypes::Surface:
		case SNOTypes::Textures:
		{
			filePathName = "Textures";
			fileExtension = ".tex";
		}
		break;

		default:
			break;
	}

	filePathName += "\\";
	filePathName += fileName;
	filePathName += fileExtension;

	return filePathName;
}

bool SNOManager::linkPatches(const QString &archiveName, HANDLE hArcive, const QString &clientMPQPath)
{
	if(m_bInitialized)
		return false;

	//Maybe there is a better way to do this.. not sure if Stormlib works the same as the client
	//I should be able to load patches once and link them, not open them multiple times for each of the main MPQs
	//This is also static to the D3 client location.. maybe TODO fix it? eh?

	char c = 0;

	//Core Archives
	if(archiveName == "ClientData.mpq" || archiveName == "CoreData.mpq" || archiveName == "Texture.mpq")
	{
		QString base = "/base/";
		QString baseCache = "/Cache/base/";
		QDir dirBase(clientMPQPath + base);
		dirBase.setFilter(QDir::Files);
		QDir dirBaseCache(clientMPQPath + baseCache);
		dirBaseCache.setFilter(QDir::Files);

		QStringList slBase = dirBase.entryList();
		QStringList slBaseCache = dirBaseCache.entryList();

		QString currentPath;

		for(int i = 0; i < slBase.size(); i++)
		{
			currentPath = clientMPQPath + base + slBase[i];
			if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), 0, 0))
			{
				int temp = 0;
			}
		}

		for(int i = 0; i < slBaseCache.size(); i++)
		{
			currentPath = clientMPQPath + baseCache + slBaseCache[i];
			if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), 0, 0))
			{
				int temp = 0;
			}
		}

		return true;
	}

	////Locale Archives
	////TODO: dynamic locale
	//if(archiveName == "enUS_Audio.mpq" || archiveName == "Sound.mpq" || archiveName == "enUS_Text.mpq")
	//{
	//	QString locale = "/enUS/";
	//	QString localeCache = "/Cache/enUS/";
	//	QDir dirLocale(clientMPQPath + locale);
	//	dirLocale.setFilter(QDir::Files);
	//	QDir dirLocaleCache(clientMPQPath + localeCache);
	//	dirLocaleCache.setFilter(QDir::Files);

	//	QStringList slLocale = dirLocale.entryList();
	//	QStringList slLocaleCache = dirLocaleCache.entryList();

	//	QString currentPath;

	//	for(int i = 0; i < slLocale.size(); i++)
	//	{
	//		currentPath = clientMPQPath + locale + slLocale[i];
	//		if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), "\\", 0))
	//		{
	//			int temp = 0;
	//		}

	//		currentPath = clientMPQPath + localeCache + slLocaleCache[i];
	//		if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), 0, 0))
	//		{
	//			int temp = 0;
	//		}
	//	}

	//	return true;
	//}

	////Platform(?) Archives
	////TODO: dynamic? need mac files?
	//if(archiveName == "HLSLShaders.mpq")
	//{
	//	QString platformWin = "/Win/";
	//	QString platformWinCache = "/Cache/Win/";
	//	QDir dirPlatformWin(clientMPQPath + platformWin);
	//	dirPlatformWin.setFilter(QDir::Files);
	//	QDir dirPlatformWinCache(clientMPQPath + platformWinCache);
	//	dirPlatformWinCache.setFilter(QDir::Files);

	//	QStringList slPlatformWin = dirPlatformWin.entryList();
	//	QStringList slPlatformWinCache = dirPlatformWinCache.entryList();

	//	QString currentPath;

	//	for(int i = 0; i < slPlatformWin.size(); i++)
	//	{
	//		currentPath = clientMPQPath + platformWin + slPlatformWin[i];
	//		if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), 0, 0))
	//		{
	//			int temp = 0;
	//		}

	//		currentPath = clientMPQPath + platformWinCache + slPlatformWinCache[i];
	//		if(!SFileOpenPatchArchive(hArcive, currentPath.toAscii().data(), 0, 0))
	//		{
	//			int temp = 0;
	//		}
	//	}

	//	return true;
	//}

	return false;
}

void SNOManager::shutdown(void)
{
	if(!m_bInitialized)
		return;

	//Close and deref files
	QMapIterator<uint32, SNOFile *> iFile(m_mFileIndex);
	while(iFile.hasNext())
	{
		iFile.next();
		if(iFile.value()->hStormFile)
		{
			SFileCloseFile(iFile.value()->hStormFile);
		}
		delete iFile.value();
	}
	m_mFileIndex.clear();

	//Close and deref Archives
	QMapIterator<QString, HANDLE> iArchive(m_mArchives);
	while(iArchive.hasNext())
	{
		iArchive.next();
		if(iArchive.value())
		{
			SFileCompactArchive(iArchive.value());
		}
	}
	m_mArchives.clear();

	m_bInitialized = false;
}

SNOManager::~SNOManager(void)
{
	shutdown();
}

SNOManager *SNOManager::instance(void)
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new SNOManager;

	return m_pInstance;
}

void SNOManager::deleteInstance(void)
{
	if(m_pInstance)
		delete m_pInstance;

	m_pInstance = 0;
}
