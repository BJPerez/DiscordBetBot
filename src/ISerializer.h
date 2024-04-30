#pragma once

class LoLBetBot;

class ISerializer
{
public:
	virtual ~ISerializer() = default;

	virtual void Serialize(const LoLBetBot& bot, std::ofstream& file) const = 0;
	virtual void UnSerialize(std::ifstream& file, LoLBetBot& data) const = 0;
};