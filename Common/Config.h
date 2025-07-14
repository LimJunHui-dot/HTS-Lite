#pragma once

#include <string>
#include <map>

class Config
{
public:
	static Config& GetInstance();

	bool LoadFromFile(const std::string& filePath);
	bool SaveToFile(const std::string& filePath);

	std::string GetString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
	int GetInt(const std::string& section, const std::string& key, int defaultValue = 0);
	bool GetBool(const std::string& section, const std::string& key, bool defaultValue = false);
	double GetDouble(const std::string& section, const std::string& key, double defaultValue = 0.0);

	void SetString(const std::string& section, const std::string& key, const std::string& value);
	void SetInt(const std::string& section, const std::string& key, int value);
	void SetBool(const std::string& section, const std::string& key, bool value);
	void SetDouble(const std::string& section, const std::string& key, double value);

private:
	Config() = default;
	~Config() = default;
	Config(const Config&) = delete;
	Config& operator=(const Config&) = delete;

	std::map > m_config;

	std::string MakeKey(const std::string& section, const std::string& key);
};