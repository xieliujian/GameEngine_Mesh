

#ifndef _Log_h__
#define _Log_h__

#include "Prerequest.h"
#include "Singleton.h"
#include "MyString.h"

NAMESPACEBEGIN(GameEngine)

enum LogMessageLevel
{
	LML_TRIVIAL = 1,
	LML_NORMAL = 2,
	LML_CRITICAL = 3
};

class LogListener
{
public:
	LogListener();
	virtual ~LogListener();

	virtual void Message( const String& message, LogMessageLevel lml);
};

class Log
{
public:
	Log(const String& name);
	~Log();

	void Logger(std::string msg, LogMessageLevel lml = LML_NORMAL);

	void AddListener(LogListener* listener);

	std::ofstream mfpLog;

	typedef std::vector<LogListener*> mtLogListener;
	mtLogListener mListeners;
};

class LogManager
{
public:
	LogManager();
	~LogManager();

	static LogManager& Instance();

	Log* CreateLog( const String& name, bool defaultLog = false);

	void Logger( const String& message, LogMessageLevel lml = LML_NORMAL);
private:
	static LogManager *s_Instance;
		
	typedef std::map<String, Log*> LogList;
		LogList m_mapLog;
		 
		Log* m_pDefaultLog;
};

#define GAMEENGINE_LOG(message, lml) {LogManager::Instance().Log(message, lml);}

NAMESPACEEND

#endif