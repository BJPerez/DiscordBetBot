#include "JsonSerializer.h"

#include <fstream>

#include "Bet.h"
#include "BettorResults.h"
#include "BotData.h"
#include "Match.h"

namespace
{
	template<typename Type>
	concept HasToJson = requires(const Type& type)
	{
		JsonSerializer::ToJson(type);
	};

	template<HasToJson ContainedType>
	dpp::json::array_t ToJsonArray(const std::vector<ContainedType>& values)
	{
		dpp::json::array_t jsonArray;
		std::ranges::for_each(values,
			[&jsonArray](const ContainedType& value)
			{
				jsonArray.push_back(JsonSerializer::ToJson(value));
			}
		);

		return jsonArray;
	}

	template<typename Type>
	concept IsStringable = requires(const Type& type)
	{
		std::to_string(type);
	};

	template<IsStringable KeyType, IsStringable ValueType>
	dpp::json::array_t ToJsonArray(const std::map<KeyType, ValueType>& values)
	{
		dpp::json::array_t jsonArray;
		for (const auto& [key, value] : values)
		{
			dpp::json json;
			json[std::to_string(key)] = std::to_string(value);
			jsonArray.push_back(json);
		}
		return jsonArray;
	}

	template<typename Type>
	concept HasFromJson = requires(const dpp::json& json, Type& type)
	{
		JsonSerializer::FromJson(json, type);
	};

	template<HasFromJson ContainedType>
	void FromJsonArray(const dpp::json::array_t& jsonArray, std::vector<ContainedType>& outVector)
	{
		std::ranges::for_each(jsonArray,
			[&outVector](const dpp::json& json)
			{
				ContainedType value;
				JsonSerializer::FromJson(json, value);
				outVector.push_back(std::move(value));
			}
		);
	}

	template<typename KeyType, typename ValueType>
	void FromJsonArray(const dpp::json::array_t& jsonArray, std::map<KeyType, ValueType>& outMap)
	{
		std::ranges::for_each(jsonArray,
			[&outMap](const dpp::json& json)
			{
				for (const auto& item : json.items()) // in our case we have only one item by json object
				{
					const std::string keyStr = item.key();
					const std::string valueStr = item.value();
					outMap[static_cast<unsigned int>(std::stoul(keyStr))] = static_cast<unsigned int>(std::stoul(valueStr));
				}
			}
		);
	}
}

dpp::json JsonSerializer::ToJson(const Match& match)
{
	dpp::json json;
	json["MatchID"] = match.GetId();
	json["TeamAName"] = match.GetTeamAName();
	json["TeamBName"] = match.GetTeamBName();
	json["BoSize"] = match.GetBoSize();

	return json;
}

dpp::json JsonSerializer::ToJson(const Bet& bet)
{
	dpp::json json;
	json["MatchID"] = bet.GetMatchId();
	json["TeamAScore"] = bet.GetScore().m_TeamAScore;
	json["TeamBScore"] = bet.GetScore().m_TeamBScore;
	json["BettorName"] = bet.GetBettorName();

	return json;
}

dpp::json JsonSerializer::ToJson(const BettorResults& bettorResults)
{
	dpp::json json;
	json["BettorName"] = bettorResults.GetBettorName();
	json["PerfectBetsByBoSize"] = ToJsonArray(bettorResults.GetPerfectBetsByBoSize());
	json["CorrectBetsByBoSize"] = ToJsonArray(bettorResults.GetCorrectBetsByBoSize());

	return json;
}

dpp::json JsonSerializer::ToJson(const BotData& data)
{
	dpp::json json;
	json["Matches"] = ToJsonArray(data.GetIncomingMatches());
	json["Bets"] = ToJsonArray(data.GetBets());
	json["BettorsResults"] = ToJsonArray(data.GetBettorsResults());

	return json;
}

void JsonSerializer::Serialize(const BotData& data, std::ofstream& file) const
{
	const dpp::json botAsJson = ToJson(data);
	file << botAsJson;
}

void JsonSerializer::UnSerialize(std::ifstream& file, BotData& data) const
{
	dpp::json dataAsJson;
	file >> dataAsJson;

	FromJson(dataAsJson, data);
}

void JsonSerializer::FromJson(const dpp::json& json, Match& outMatch)
{
	outMatch.SetId(json["MatchID"]);
	outMatch.SetTeamAName(json["TeamAName"]);
	outMatch.SetTeamBName(json["TeamBName"]);
	outMatch.SetBoSize(json["BoSize"]);
}

void JsonSerializer::FromJson(const dpp::json& json, Bet& outBet)
{
	outBet.SetMatchId(json["MatchID"]);
	outBet.SetScore(MatchScore{ json["TeamAScore"], json["TeamBScore"] });
	outBet.SetBettorName(json["BettorName"]);
}

void JsonSerializer::FromJson(const dpp::json& json, BettorResults& outResults)
{
	outResults.SetBettorName(json["BettorName"]);

	std::map<unsigned int, unsigned int> perfectBetsByBoSize;
	FromJsonArray(json["PerfectBetsByBoSize"], perfectBetsByBoSize);
	outResults.SetPerfectBetsByBoSize(perfectBetsByBoSize);

	std::map<unsigned int, unsigned int> correctBetsByBoSize;
	FromJsonArray(json["CorrectBetsByBoSize"], correctBetsByBoSize);
	outResults.SetCorrectBetsByBoSize(correctBetsByBoSize);
}

void JsonSerializer::FromJson(const dpp::json& json, BotData& outData)
{
	std::vector<Match> incomingMatches;
	FromJsonArray(json["Matches"], incomingMatches);
	outData.SetIncomingMatches(incomingMatches);

	std::vector<Bet> bets;
	FromJsonArray(json["Bets"], bets);
	outData.SetBets(bets);

	std::vector<BettorResults> results;
	FromJsonArray(json["BettorsResults"], results);
	outData.SetBettorResults(results);
}

