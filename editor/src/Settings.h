#ifndef _SETTINGS_H_
#define _SETTINGS_H_
#include <QSettings>

class Settings : public QSettings
{
	private:
		static Settings *m_pInstance;

		int m_Status;

		Settings(void);
		Settings(const Settings &ref){}
		Settings &operator=(const Settings &ref){}

	public:
		~Settings(void);

		static Settings *instance(void);
		static void deleteInstance(void);

		bool initialize(void);
		void shutdown(void);
};

#endif //#ifndef _SETTINGS_H_
