#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <QString>
#include <QFile>
#include <QMap>
#include "Logwindow.h"

class Logger
{
	private:
		static Logger *m_pInstance;

		LogWindow *m_pLogWindow;

		QMap<QString, QFile *> m_FileMap;

		bool m_initialized;

		Logger(void);
		Logger(const Logger &ref){}
		Logger &operator=(const Logger &ref){}

	public:
		~Logger(void);

		static Logger *instance(void);
		static void deleteInstance(void);

		bool initialize(LogWindow *pLogWindow);
		void shutdown(void);

		bool logString(const QString &messageString, bool bNewLine = true, const QString &fileName = "logs/log.txt", const bool append = true);
};

#endif //#ifndef _LOGGER_H_
