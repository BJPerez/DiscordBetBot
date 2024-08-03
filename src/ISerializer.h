#pragma once

#include "LockableDataAccessors.h"

class ICommandReceiver;

class ISerializer
{
public:
	virtual ~ISerializer() = default;

	virtual void Serialize(const DataReader<ICommandReceiver>& dataReader, std::ofstream& file) const = 0;
	virtual void UnSerialize(std::ifstream& file, const DataWriter<ICommandReceiver>& dataWriter) const = 0;
};