#pragma once

#include <memory>
#include <string>

class LoLBetBot;
class ISerializer;

class SaveManager
{
public:
	explicit SaveManager(std::string filePath, LoLBetBot& bot);

	void Save() const;
	void Load() const;

private:
	std::unique_ptr<ISerializer> m_Serializer;
	std::string m_SaveFilePath;
	LoLBetBot& m_Data;
};
