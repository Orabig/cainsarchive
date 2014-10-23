#ifndef _LOGWINDOW_H_
#define _LOGWINDOW_H_
#include <string>

class LogWindow
{
	public:
		virtual ~LogWindow(void){}

		virtual bool initialize(void) = 0;
		virtual void shutdown(void) = 0;

		virtual void showMessage(const std::string &Message) = 0;
};

#endif //#ifndef _LOGWINDOW_H_
