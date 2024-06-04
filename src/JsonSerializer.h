#pragma once

#include <dpp/json.h>

#include "BettorResults.h"
#include "ISerializer.h"

class Bet;
class BotData;
class Match;

class JsonSerializer final : public ISerializer
{
public:
	void Serialize(const BotData& data, std::ofstream& file) const final;
	void UnSerialize(std::ifstream& file, BotData& data) const final;

	[[nodiscard]] static dpp::json ToJson(const Match& match);
	[[nodiscard]] static dpp::json ToJson(const Bet& bet);
	[[nodiscard]] static dpp::json ToJson(const BettorResults& bettorResults);
	[[nodiscard]] static dpp::json ToJson(const BotData& data);
	[[nodiscard]] static dpp::json ToJson(const BettorResults::ResultsByBoSize& results);

	static void FromJson(const dpp::json& json, Match& outMatch);
	static void FromJson(const dpp::json& json, Bet& outBet);
	static void FromJson(const dpp::json& json, BettorResults& outResults);
	static void FromJson(const dpp::json& json, BotData& outData);
	static void FromJson(const dpp::json& json, BettorResults::ResultsByBoSize& outResults);
};
