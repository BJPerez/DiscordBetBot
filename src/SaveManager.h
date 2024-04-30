#pragma once

#include <memory>
#include <string>

class BotData;
class ISerializer;

class SaveManager
{
public:
	explicit SaveManager(std::string filePath, BotData& data);

	void Save() const;
	void Load() const;

private:
	std::unique_ptr<ISerializer> m_Serializer;
	std::string m_SaveFilePath;
	BotData& m_Data;
};
