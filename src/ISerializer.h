#pragma once

class BotData;

class ISerializer
{
public:
	virtual ~ISerializer() = default;

	virtual void Serialize(const BotData& data, std::ofstream& file) const = 0;
	virtual void UnSerialize(std::ifstream& file, BotData& data) const = 0;
};