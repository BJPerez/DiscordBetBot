#include "BotData.h"

#include <algorithm>

void BotData::AddMatch(std::string teamAName, std::string teamBName, const unsigned int boSize)
{
	m_IncomingMatches.emplace_back(std::move(teamAName), std::move(teamBName), boSize);
}

void BotData::AddBet(const unsigned int matchId, const MatchScore& matchResult, std::string bettorName)
{
	m_Bets.emplace_back(matchId, matchResult, std::move(bettorName));
}

std::vector<std::reference_wrapper<const Bet>> BotData::GetBetsLinkedToMatch(const unsigned int matchId) const
{
	std::vector<std::reference_wrapper<const Bet>>  betsLinkedToMatch;
	std::ranges::for_each(m_Bets,
		[&betsLinkedToMatch, matchId](const Bet& bet)
		{
			if (bet.GetMatchId() == matchId)
			{
				betsLinkedToMatch.emplace_back(bet);
			}
		}
	);

	return betsLinkedToMatch;
}

void BotData::EraseBetsLinkedToMatch(const unsigned int matchId)
{
	std::erase_if(m_Bets,
		[matchId](const Bet& bet)
		{
			return bet.GetMatchId() == matchId;
		}
	);
}

BettorResults& BotData::GetOrCreateBettorResults(std::string bettorName)
{
	const auto valueFound = std::ranges::find_if(m_BettorResults,
		[&bettorName](const BettorResults& results)
		{
			return results.GetBettorName() == bettorName;
		}
	);

	if (valueFound == m_BettorResults.end())
	{
		return m_BettorResults.emplace_back(std::move(bettorName));
	}
	return *valueFound;
}

void BotData::AddResult(const unsigned int matchId, const MatchScore& matchResult)
{
	const std::optional<std::reference_wrapper<const Match>> matchOpt = GetMatch(matchId);
	if (!matchOpt.has_value())
	{
		return;
	}
	const Match& match = matchOpt.value().get();

	std::vector<std::reference_wrapper<const Bet>> betsLinkedToMatch = GetBetsLinkedToMatch(matchId);
	std::ranges::for_each(betsLinkedToMatch,
		[this, &matchResult, &match](const Bet& bet)
		{
			BettorResults& results = GetOrCreateBettorResults(bet.GetBettorName());
			if (const Bet::Result betResult = bet.EvaluateBet(matchResult); 
				betResult != Bet::Result::IncorrectBet)
			{
				results.AddResult(betResult == Bet::Result::PerfectBet, match.GetBoSize());
			}
		}
	);

	SortResults();

	EraseBetsLinkedToMatch(matchId);
	m_IncomingMatches.erase(std::ranges::find(m_IncomingMatches, match));
}

std::optional<std::reference_wrapper<const Match>> BotData::GetMatch(const unsigned int matchId) const
{
	if (matchId == Match::INVALID_ID)
	{
		return std::nullopt;
	}

	const auto matchIt = std::ranges::find_if(m_IncomingMatches,
		[matchId](const Match& match)
		{
			return match.GetId() == matchId;
		}
	);

	if (matchIt == m_IncomingMatches.end())
	{
		return std::nullopt;
	}
	return std::optional<std::reference_wrapper<const Match>>(*matchIt);
}

std::optional<std::reference_wrapper<const Bet>> BotData::GetBet(const unsigned int matchId, const std::string& bettorName) const
{
	return const_cast<BotData*>(this)->GetBet(matchId, bettorName);
}

std::optional<std::reference_wrapper<Bet>> BotData::GetBet(const unsigned int matchId, const std::string& bettorName) 
{
	if (matchId == Match::INVALID_ID || bettorName.empty())
	{
		return std::nullopt;
	}

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

void BotData::ModifyBet(const unsigned int matchId, const MatchScore& matchResult, const std::string& bettorName)
{
	if (const std::optional<std::reference_wrapper<Bet>> bet = GetBet(matchId, bettorName))
	{
		bet.value().get().SetScore(matchResult);
	}
}

void BotData::SetBettorResults(std::vector<BettorResults> results)
{
	m_BettorResults = std::move(results);
	SortResults();
}

void BotData::SortResults()
{
	std::ranges::sort(m_BettorResults,
		[](const BettorResults& a, const BettorResults& b)
		{
			return a.GetScore() > b.GetScore();
		}
	);
}

std::vector<std::reference_wrapper<const Bet>> BotData::GetBetsWithFilter(const BetFilter& filter) const
{
	std::vector<std::reference_wrapper<const Bet>> bets;

	std::ranges::for_each(m_Bets,
		[&bets, &filter](const Bet& bet)
		{
			if (filter(bet))
			{
				bets.emplace_back(bet);
			}
		}
	);

	return bets;
}

std::vector<std::reference_wrapper<Bet>> BotData::GetBetsWithFilter(const BetFilter& filter)
{
	std::vector<std::reference_wrapper<Bet>> bets;

	std::ranges::for_each(m_Bets,
		[&bets, &filter](Bet& bet)
		{
			if (filter(bet))
			{
				bets.emplace_back(bet);
			}
		}
	);

	return bets;
}

std::vector<std::reference_wrapper<const Bet>> BotData::GetBetsOnMatch(const unsigned int matchId) const
{
	const auto filter =
		[matchId](const Bet& bet)
		{
			return bet.GetMatchId() == matchId;
		};

	return GetBetsWithFilter(filter);
}
