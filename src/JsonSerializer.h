#pragma once

#include <dpp/json.h>

#include "ISerializer.h"

class LoLBetBot;
class BettorResults;
class Bet;
class Match;

class JsonSerializer final : public ISerializer
{
public:
	void Serialize(const LoLBetBot& bot, std::ofstream& file) const final;
	void UnSerialize(std::ifstream& file, LoLBetBot& data) const final;

	[[nodiscard]] static dpp::json ToJson(const Match& match);
	[[nodiscard]] static dpp::json ToJson(const Bet& bet);
	[[nodiscard]] static dpp::json ToJson(const BettorResults& bettorResults);
	[[nodiscard]] static dpp::json ToJson(const LoLBetBot& bot);

	static void FromJson(const dpp::json& json, Match& outMatch);
	static void FromJson(const dpp::json& json, Bet& outBet);
	static void FromJson(const dpp::json& json, BettorResults& outResults);
	static void FromJson(const dpp::json& json, LoLBetBot& outBotData);
};
