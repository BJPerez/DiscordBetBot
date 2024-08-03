#include "SaveManager.h"

#include <fstream>

#include "ICommandReceiver.h"
#include "JsonSerializer.h"

SaveManager::SaveManager(std::string filePath): m_SaveFilePath(std::move(filePath))
{
	m_Serializer = std::make_unique<JsonSerializer>();
}

void SaveManager::Save(const DataReader<ICommandReceiver>& dataReader) const
{
	std::ofstream os(m_SaveFilePath, std::ofstream::out);
	m_Serializer->Serialize(dataReader, os);
	os.close();
}

void SaveManager::Load(const DataWriter<ICommandReceiver>& dataWriter) const
{
	if (std::ifstream is(m_SaveFilePath); is.good()) // does file exists ?
	{
		m_Serializer->UnSerialize(is, dataWriter);
		is.close();
	}
}
