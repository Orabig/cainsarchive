#include "Settings.h"

Settings *Settings::m_pInstance = 0;

Settings::Settings(void)
:QSettings("CainsAide", "Settings")
{
    m_Status = -1;
}

Settings::~Settings(void)
{
    shutdown();
}

Settings *Settings::instance(void)
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new Settings;

	return m_pInstance;
}

void Settings::deleteInstance(void)
{
	if(m_pInstance)
		delete m_pInstance;

	m_pInstance = 0;
}

bool Settings::initialize(void)
{
    if(m_Status >= 0)
	return true;

    if(!value("Initialized").toBool())
    {
		//Initialize settings
		setValue("Application", "Diablo III.exe");
		setValue("FileLocale", "C:\\Program Files (x86)\\Diablo III");

		setValue("Initialized", true);
		sync();
    }

    m_Status = 0;

    return true;
}

void Settings::shutdown(void)
{
    if(m_Status < 0)
	return;

    m_Status = -1;
}
