#include "LogWindowWin32.h"
#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

LogWindowWin32 *LogWindowWin32::m_pInstance = 0;

LogWindowWin32::LogWindowWin32(void)
{
	m_Initialized = false;
}

LogWindowWin32::~LogWindowWin32(void)
{
	shutdown();
}

LogWindowWin32 *LogWindowWin32::instance(void)
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new LogWindowWin32;

	return m_pInstance;
}

void LogWindowWin32::deleteInstance(void)
{
	if(m_pInstance)
		delete m_pInstance;

	m_pInstance = NULL;
}

bool LogWindowWin32::initialize(void)
{
	if(m_Initialized)
		return m_Initialized;

	AllocConsole();
	SetConsoleTitleA("Editor Window - Win32");

	int hConHandle;
	long lStdHandle;
	FILE *fp;

	//redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	//redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	//redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	showMessage("Editor log window initialized.\n");

	m_Initialized = true;

	return m_Initialized;
}

void LogWindowWin32::shutdown(void)
{
	if(!m_Initialized)
		return;

	m_Initialized = false;
}

void LogWindowWin32::showMessage(const std::string &Message)
{
	printf(Message.c_str());
}
