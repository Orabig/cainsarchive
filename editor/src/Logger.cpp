#include "Logger.h"
#include <QDir>

Logger *Logger::m_pInstance = 0;

Logger::Logger(void)
{
	m_pLogWindow = 0;
	m_initialized = false;
}

Logger::~Logger(void)
{
	shutdown();
}

Logger *Logger::instance(void)
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new Logger;

	return m_pInstance;
}

void Logger::deleteInstance(void)
{
	if(m_pInstance)
		delete m_pInstance;

	m_pInstance = 0;
}

bool Logger::initialize(LogWindow *pLogWindow)
{
	if(m_initialized)
		return m_initialized;

	m_pLogWindow = pLogWindow;

	QDir appDir;

	if(!appDir.exists("logs"))
	{
		appDir.mkdir("logs");
	}
	
	m_initialized = true;

	//logString("Logger initialized.\n");

	return m_initialized;
}

void Logger::shutdown(void)
{
	if(!m_initialized)
		return;

	if(!m_FileMap.isEmpty())
	{
		QMapIterator<QString, QFile *> i(m_FileMap);
		while(1)
		{
			if(i.value())
			{
				i.value()->close();
				delete i.value();
			}

			if(!i.hasNext())
				break;

			i.next();
		}

		m_FileMap.clear();
	}

	m_initialized = false;
}

bool Logger::logString(const QString &messageString, bool bNewLine, const QString &fileName, const bool append)
{
	if(!m_initialized)
		return false;

	QFile *pLogFile = 0;

	if(!m_FileMap.contains(fileName))
	{
		QFile *pNewLogFile = new QFile(fileName);
		pNewLogFile->open(QIODevice::WriteOnly);
		m_FileMap.insert(fileName, pNewLogFile);
		pLogFile = pNewLogFile;
		pNewLogFile;
	}
	else
	{
		pLogFile = m_FileMap.value(fileName);
	}

	if(pLogFile == 0)
		return false;

	if(!append)
	{
		pLogFile->close();
		pLogFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
	}

	pLogFile->write(messageString.toAscii().data(), messageString.size());

	if(bNewLine)
	{
		char crlf[3];
		crlf[0] = char(13);
		crlf[1] = char(10);
		crlf[2] = 0;

		pLogFile->write(crlf, 2);
	}

	pLogFile->flush();

//	if(m_pLogWindow != 0)
//		m_pLogWindow->showMessage(String);

	return true;
}
