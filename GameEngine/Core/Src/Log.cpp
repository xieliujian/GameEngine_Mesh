

#include "Log.h"
#include <time.h>
#include <iostream>
#include <iomanip>

NAMESPACEBEGIN(GameEngine)

// -----------------
// LogListener
// -----------------
LogListener::LogListener()
{

}

LogListener::~LogListener()
{

}

void LogListener::Message( const String& message, LogMessageLevel lml)
{
}

// -----------------
// Log
// -----------------
Log::Log(const String& name)
{
	mfpLog.open(name.c_str());
}

Log::~Log()
{
	mfpLog.close();
}

void Log::Logger(std::string message, LogMessageLevel lml)
{
	for( mtLogListener::iterator i = mListeners.begin(); i != mListeners.end(); ++i )
		(*i)->Message( message, lml);

	std::string desc;

	switch (lml)
	{
	case LML_TRIVIAL:
		desc = "Trivial";
		break;
	case LML_NORMAL:
		desc = "Normal";
		break;
	case LML_CRITICAL:
		desc = "Critical";
		break;
	}

	// Write time into log
	struct tm *pTime = NULL;
	time_t ctTime; time(&ctTime);
	localtime_s(pTime, &ctTime );
	mfpLog << desc << " " << std::setw(2) << std::setfill('0') << pTime->tm_hour
		<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
		<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec 
		<< ": " << message << std::endl;

	// Flush stcmdream to ensure it is written (incase of a crash, we need log to be up to date)
	mfpLog.flush();
}

void Log::AddListener(LogListener* listener)
{
	mListeners.push_back(listener);
}

// -----------------
// LogManager
// -----------------
//template <> LogManager* CSingleton<LogManager>::msInstance = NULL;
//template <> const char* CSingleton<LogManager>::mClassTypeName("LogManager");

LogManager* LogManager::s_Instance = NULL;

LogManager::LogManager()
	:m_pDefaultLog(NULL)
{
	s_Instance = this;
	m_mapLog.clear();
}

LogManager::~LogManager()
{
	LogList::iterator i;
	for (i = m_mapLog.begin(); i != m_mapLog.end(); ++i)
	{
		SAFE_DELETE(i->second);
	}
}

LogManager& LogManager::Instance()
{
	return *s_Instance;
}

Log* LogManager::CreateLog( const String& name, bool defaultLog)
{
	Log* newLog = new Log(name);

	if( !m_pDefaultLog || defaultLog )
	{
		m_pDefaultLog = newLog;
	}

	m_mapLog.insert( LogList::value_type( name, newLog ) );

	return newLog;
}

void LogManager::Logger( const String& message, LogMessageLevel lml)
{
	if (m_pDefaultLog)
		m_pDefaultLog->Logger(message, lml);
}

NAMESPACEEND