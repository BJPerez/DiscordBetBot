#include "JsonSerializer.h"

#include <fstream>

#include "Bet.h"
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
}

dpp::json JsonSerializer::ToJson(const Match& match)
{
	dpp::json json;
	json["MatchID"] = match.GetId();
	json["TeamAName"] = match.GetTeamAName();
	json["TeamBName"] = match.GetTeamBName();
	json["BoSize"] = match.GetBoSize();
	if (match.IsPlayed())
	{
		json["TeamAScore"] = match.GetResult().m_TeamAScore;
		json["TeamBScore"] = match.GetResult().m_TeamBScore;
	}
	json["DateTime"] = match.GetDateAndTime().ToString();

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

dpp::json JsonSerializer::ToJson(const BettorResults::ResultsByBoSize& results)
{
	dpp::json json;
	json["BoSize"] = results.m_BoSize;
	json["PerfectBets"] = results.m_PerfectBetsCount;
	json["CorrectBets"] = results.m_CorrectBetsCount;
	json["IncorrectBets"] = results.m_IncorrectBetsCount;
	json["Score"] = results.m_Score;

	return json;
}

dpp::json JsonSerializer::ToJson(const DataReader<ICommandReceiver>& dataReader)
{
	dpp::json json;
	json["Matches"] = ToJsonArray(dataReader->GetAllMatches());
	json["Bets"] = ToJsonArray(dataReader->GetBets());

	return json;
}

void JsonSerializer::Serialize(const DataReader<ICommandReceiver>& dataReader, std::ofstream& file) const
{
	const dpp::json botAsJson = ToJson(dataReader);
	file << botAsJson;
}

void JsonSerializer::UnSerialize(std::ifstream& file, const DataWriter<ICommandReceiver>& dataWriter) const
{
	dpp::json dataAsJson;
	file >> dataAsJson;

	FromJson(dataAsJson, dataWriter);
}

void JsonSerializer::FromJson(const dpp::json& json, Match& outMatch)
{
	outMatch.SetId(json["MatchID"]);
	outMatch.SetTeamAName(json["TeamAName"]);
	outMatch.SetTeamBName(json["TeamBName"]);
	outMatch.SetBoSize(json["BoSize"]);
	if (json.contains("TeamAScore") && json.contains("TeamBScore"))
	{
		outMatch.SetResult({ json["TeamAScore"], json["TeamBScore"] });
	}
	const DateAndTime dateTime{ json["DateTime"] };
	outMatch.SetDateAndTime(dateTime);
}

void JsonSerializer::FromJson(const dpp::json& json, Bet& outBet)
{
	outBet.SetMatchId(json["MatchID"]);
	outBet.SetScore(MatchScore{ json["TeamAScore"], json["TeamBScore"] });
	outBet.SetBettorName(json["BettorName"]);
}

void JsonSerializer::FromJson(const dpp::json& json, BettorResults::ResultsByBoSize& outResults)
{
	outResults.m_BoSize = json["BoSize"];
	outResults.m_PerfectBetsCount = json["PerfectBets"];
	outResults.m_CorrectBetsCount = json["CorrectBets"];
	outResults.m_IncorrectBetsCount = json["IncorrectBets"];
	outResults.m_Score = json["Score"];
}

void JsonSerializer::FromJson(const dpp::json& json, const DataWriter<ICommandReceiver>& dataWriter)
{
	std::vector<Match> incomingMatches;
	FromJsonArray(json["Matches"], incomingMatches);
	dataWriter->SetMatches(std::move(incomingMatches));

	std::vector<Bet> bets;
	FromJsonArray(json["Bets"], bets);
	dataWriter->SetBets(std::move(bets));
}

