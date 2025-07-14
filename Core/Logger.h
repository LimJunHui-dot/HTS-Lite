#pragma once


#include <string>
#include <mutex>


// 로그 기록 및 디버깅 지원
class Logger
{
public:
	static Logger& GetInstance();

	bool Initialize(const std::string& logDirectory);
	void shutdown();

	void LogDebug(const std::string& message);
	void LogInfo(const std::string& message);
	void LogWarning(const std::string& message);
	void LogError(const std::string& message);
	void LogFatal(const std::string& message);

	void SetLogLevel(LogLevel level) { m_logLevel = level; }
	LogLevel GetLogLevel() const { return m_logLevel; }

private:
	Logger() = default;
	~Logger() = default;
	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

	void WriteLog(LogLevel level, const std::string& message);
	std::string GetLogLevelString(LogLevel level);
	std::string GetCurrentFileName();
	bool CreateLogDirectory(const std::string& directory);

	std::ofstream m_logFile;
	std::mutex m_logMutex;
	LogLevel m_logLevel = LogLevel::INFO;
	std::string m_logDirectory;
	std::string m_currentLogFile;
	bool m_initialized = false;
};

