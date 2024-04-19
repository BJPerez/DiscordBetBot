#include "LoLBetBot.h"

#include <algorithm>

void LoLBetBot::AddMatch(std::string teamAName, std::string teamBName, const unsigned int boSize)
{
	m_IncomingMatches.emplace_back(std::move(teamAName), std::move(teamBName), boSize);
}

void LoLBetBot::AddBet(const unsigned int matchId, const MatchScore& matchResult, std::string bettorName)
{
	m_Bets.emplace_back(matchId, matchResult, std::move(bettorName));
}

std::vector<std::reference_wrapper<const Bet>> LoLBetBot::GetBetsLinkedToMatch(const unsigned int matchId) const
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

void LoLBetBot::EraseBetsLinkedToMatch(const unsigned int matchId)
{
	std::erase_if(m_Bets,
		[matchId](const Bet& bet)
		{
			return bet.GetMatchId() == matchId;
		}
	);
}

BettorResults& LoLBetBot::GetOrCreateBettorResults(std::string bettorName)
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

void LoLBetBot::AddResult(const unsigned int matchId, const MatchScore& matchResult)
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

	std::ranges::sort(m_BettorResults, 
		[](const BettorResults& a, const BettorResults& b)
		{
			return a.GetScore() > b.GetScore();
		}
	);

	EraseBetsLinkedToMatch(matchId);
	m_IncomingMatches.erase(std::ranges::find(m_IncomingMatches, match));
}

std::optional<std::reference_wrapper<const Match>> LoLBetBot::GetMatch(const unsigned int matchId) const
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

std::optional<std::reference_wrapper<const Bet>> LoLBetBot::GetBet(const unsigned int matchId, const std::string& bettorName) const
{
	return const_cast<LoLBetBot*>(this)->GetBet(matchId, bettorName);
}

std::optional<std::reference_wrapper<Bet>> LoLBetBot::GetBet(const unsigned int matchId, const std::string& bettorName) 
{
	if (matchId == Match::INVALID_ID || bettorName.empty())
	{
		return std::nullopt;
	}

	const auto betIt = std::ranges::find_if(m_Bets,
		[matchId, &bettorName](const Bet& bet)
		{
			return bet.GetMatchId() == matchId && bettorName == bet.GetBettorName();
		}
	);

	if (betIt == m_Bets.end())
	{
		return std::nullopt;
	}
	return std::reference_wrapper<Bet>(*betIt);
}

void LoLBetBot::ModifyBet(const unsigned int matchId, const MatchScore& matchResult, const std::string& bettorName)
{
	if (const std::optional<std::reference_wrapper<Bet>> bet = GetBet(matchId, bettorName))
	{
		bet.value().get().SetScore(matchResult);
	}
}
