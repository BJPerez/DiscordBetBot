#include "BotData.h"

#include "BotDataExceptions.h"

namespace
{
	MatchScore EvaluateScore(const ICommandReceiver::AddResultParams& params, const Match& match)
	{
		if (params.m_TeamAName.has_value() && params.m_TeamBName.has_value())
		{
			const std::string& userTeamAName = params.m_TeamAName.value();
			const std::string& userTeamBName = params.m_TeamBName.value();

			if (match.GetTeamAName() == userTeamAName && match.GetTeamBName() == userTeamBName)
			{
				return { params.m_TeamAScore, params.m_TeamBScore };
			}
			else if (match.GetTeamAName() == userTeamBName && match.GetTeamBName() == userTeamAName)
			{
				return { params.m_TeamBScore, params.m_TeamAScore };
			}
			else
			{
				throw UnmatchingTeamNameException{ userTeamAName, userTeamBName, match.GetTeamAName(), match.GetTeamBName() };
			}
		}

		return { params.m_TeamAScore, params.m_TeamBScore };
	}
}

void BotData::AddMatch(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int boSize)
{
	if (matchId.has_value() && (matchId.value().empty() || matchId.value() == Match::INVALID_ID))
	{
		throw InvalidMatchIdException(std::move(matchId.value()));
	}

	if (matchId.has_value() && HasMatch(matchId.value()))
	{
		throw MatchIdUnavailableException(std::move(matchId.value()));
	}

	if (teamAName.empty() || teamBName.empty())
	{
		throw InvalidTeamNameException(std::move(teamAName), std::move(teamBName));
	}

	if (boSize % 2 != 1)
	{
		throw InvalidBoSizeException(boSize);
	}

	m_Matches.emplace_back(std::move(matchId), std::move(teamAName), std::move(teamBName), boSize);
}

void BotData::AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName)
{
	if (!HasMatch(matchId))
	{
		throw MatchNotFoundException(std::move(matchId)); 
	}

	const Match& match = GetMatch(matchId);
	if (match.IsPlayed())
	{
		throw MatchAlreadyPlayedException(std::move(matchId));
	}

	if (!match.IsValidScore(matchResult))
	{
		throw InvalidScoreException(match.GetBoSize(), matchResult);
	}

	if (bettorName.empty())
	{
		throw InvalidBettorNameException(std::move(bettorName));
	}

	if (HasBet(matchId, bettorName))
	{
		throw BetAlreadyExistException(matchId, std::move(bettorName));
	}

	m_Bets.emplace_back(std::move(matchId), matchResult, std::move(bettorName));
}

void BotData::AddResult(const AddResultParams& params)
{
	if (!HasMatch(params.m_MatchId))
	{
		throw MatchNotFoundException(params.m_MatchId);
	}

	Match& match = GetMatch(params.m_MatchId);
	if (match.IsPlayed())
	{
		throw MatchAlreadyPlayedException(params.m_MatchId);
	}

	match.SetResult(EvaluateScore(params, match));
}

const Match& BotData::GetMatch(const std::string& matchId) const
{
	return const_cast<BotData*>(this)->GetMatch(matchId);
}

Match& BotData::GetMatch(const std::string& matchId)
{
	if (matchId == Match::INVALID_ID || matchId.empty())
	{
		throw InvalidMatchIdException(matchId);
	}

	for (Match& match : m_Matches)
	{
		if (match.GetId() == matchId)
		{
			return match;
		}
	}

	throw MatchNotFoundException(matchId);
}

const Bet& BotData::GetBet(const std::string& matchId, const std::string& bettorName) const
{
	return const_cast<BotData*>(this)->GetBet(matchId, bettorName);
}

Bet& BotData::GetBet(const std::string& matchId, const std::string& bettorName) 
{
	if (matchId.empty() || matchId == Match::INVALID_ID)
	{
		throw InvalidMatchIdException(matchId);
	}

	if (bettorName.empty())
	{
		throw InvalidBettorNameException(bettorName);
	}

	for (Bet& bet : m_Bets)
	{
		if (bet.GetMatchId() == matchId && bet.GetBettorName() == bettorName)
		{
			return bet;
		}
	}

	throw BetNotFoundException(matchId, bettorName);
}

void BotData::ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName)
{
	if (!HasBet(matchId, bettorName))
	{
		throw BetNotFoundException(matchId, bettorName);
	}

	const Match& match = GetMatch(matchId);
	if (match.IsPlayed())
	{
		throw MatchAlreadyPlayedException(matchId);
	}

	if (!match.IsValidScore(matchResult))
	{
		throw InvalidScoreException(match.GetBoSize(), matchResult);
	}

	GetBet(matchId, bettorName).SetScore(matchResult);
}

std::vector<std::reference_wrapper<const Bet>> BotData::GetBetsOnMatch(const std::string& matchId) const
{
	if (!HasMatch(matchId))
	{
		throw MatchNotFoundException(matchId);
	}

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

bool BotData::HasMatch(const std::string& matchId) const
{
	if (matchId.empty() || matchId == Match::INVALID_ID)
	{
		throw InvalidMatchIdException(matchId);
	}

	const auto filter =
		[&matchId](const Match& match)
		{
			return match.GetId() == matchId;
		};

	if (const std::vector<std::reference_wrapper<const Match>> matches = GetMatchesWithFilter(filter); 
		matches.empty())
	{
		return false;
	}
	return true; 
}

[[nodiscard]] bool BotData::HasBet(const std::string& matchId, const std::string& bettorName) const
{
	if (!HasMatch(matchId))
	{
		throw MatchNotFoundException(matchId);
	}

	if (bettorName.empty())
	{
		throw InvalidBettorNameException(bettorName);
	}

	const auto filter =
		[&matchId, &bettorName](const Bet& bet)
		{
			return bet.GetMatchId() == matchId && bet.GetBettorName() == bettorName;
		};

	if (const std::vector<std::reference_wrapper<const Bet>> bets = GetBetsWithFilter(filter);
		bets.empty())
	{
		return false;
	}
	return true;
}