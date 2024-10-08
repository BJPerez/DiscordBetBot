#include "BotData.h"

void BotData::AddMatch(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int boSize)
{
	m_Matches.emplace_back(std::move(matchId), std::move(teamAName), std::move(teamBName), boSize);
}

void BotData::AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName)
{
	m_Bets.emplace_back(std::move(matchId), matchResult, std::move(bettorName));
}

void BotData::AddResult(const std::string& matchId, const MatchScore& matchResult)
{
	const std::optional<std::reference_wrapper<Match>> matchOpt = GetMatch(matchId);
	if (!matchOpt.has_value())
	{
		return;
	}
	Match& match = matchOpt.value().get();
	match.SetResult(matchResult);
}

std::optional<std::reference_wrapper<const Match>> BotData::GetMatch(const std::string& matchId) const
{
	return const_cast<BotData*>(this)->GetMatch(matchId);
}

std::optional<std::reference_wrapper<Match>> BotData::GetMatch(const std::string& matchId)
{
	const auto filter =
		[matchId](const Match& match)
		{
			return match.GetId() == matchId;
		};

	std::vector<std::reference_wrapper<Match>> matches = GetMatchesWithFilter(filter);
	if (matches.empty())
	{
		return std::nullopt;
	}
	return matches[0]; // Should only have one value in the vector
}

std::optional<std::reference_wrapper<const Bet>> BotData::GetBet(const std::string& matchId, const std::string& bettorName) const
{
	return const_cast<BotData*>(this)->GetBet(matchId, bettorName);
}

std::optional<std::reference_wrapper<Bet>> BotData::GetBet(const std::string& matchId, const std::string& bettorName) 
{
	const auto filter =
		[matchId, &bettorName](const Bet& bet)
		{
			return bet.GetMatchId() == matchId && bettorName == bet.GetBettorName();
		};

	std::vector<std::reference_wrapper<Bet>> bets = GetBetsWithFilter(filter);
	if (bets.empty())
	{
		return std::nullopt;
	}
	return bets[0]; // Should only have one value in the vector
}

void BotData::ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName)
{
	if (const std::optional<std::reference_wrapper<Bet>> bet = GetBet(matchId, bettorName))
	{
		bet.value().get().SetScore(matchResult);
	}
}

std::vector<std::reference_wrapper<const Bet>> BotData::GetBetsOnMatch(const std::string& matchId) const
{
	const auto filter =
		[matchId](const Bet& bet)
		{
			return bet.GetMatchId() == matchId;
		};

	return GetBetsWithFilter(filter);
}

std::vector<std::reference_wrapper<const Match>> BotData::GetIncomingMatches() const
{
	const auto filter =
		[](const Match& match)
		{
			return !match.IsPlayed();
		};

	return GetMatchesWithFilter(filter);
}

std::vector<std::reference_wrapper<const Match>> BotData::GetPastMatches() const
{
	const auto filter =
		[](const Match& match)
		{
			return match.IsPlayed();
		};

	return GetMatchesWithFilter(filter);
}