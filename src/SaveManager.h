#pragma once

#include <memory>
#include <string>

#include "LockableDataAccessors.h"

class ICommandReceiver;
class ISerializer;

class SaveManager
{
public:
	explicit SaveManager(std::string filePath);

	void Save(const DataReader<ICommandReceiver>& dataReader) const;
	void Load(const DataWriter<ICommandReceiver>& dataWriter) const;

private:
	std::unique_ptr<ISerializer> m_Serializer;
	std::string m_SaveFilePath;
};
