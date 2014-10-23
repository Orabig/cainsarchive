#ifndef _LOGWINDOWWIN32_H_
#define _LOGWINDOWWIN32_H_
#include "logwindow.h"
#include <string>

class LogWindowWin32 : public LogWindow
{
	private:
		static LogWindowWin32 *m_pInstance;

		bool m_Initialized;

		LogWindowWin32(void);
		LogWindowWin32(const LogWindowWin32 &ref){}
		LogWindowWin32 &operator=(const LogWindowWin32 &ref){}

	public:
		~LogWindowWin32(void);

		static LogWindowWin32 *instance(void);
		static void deleteInstance(void);

		bool initialize(void);
		void shutdown(void);

		void showMessage(const std::string &Message);
};

#endif //#ifndef _BOXEL_LOGWINDOWWIN32_H_
