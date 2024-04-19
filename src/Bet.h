#pragma once

#include <string>

#include "MatchScore.h"

class Bet
{
public:
	enum class Result
	{
		IncorrectBet,
		CorrectBet,
		PerfectBet
	};

	explicit Bet(const unsigned int matchId, const MatchScore& score, std::string bettorName):
		m_MatchId(matchId), m_Score{ score }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] unsigned int GetMatchId() const { return m_MatchId; }
	[[nodiscard]] const MatchScore& GetScore() const { return m_Score; }
	[[nodiscard]] const std::string& GetBettorName() const { return m_BettorName; }

	[[nodiscard]] Result EvaluateBet(const MatchScore& matchScore) const;

	void SetScore(const MatchScore& score) { m_Score = score; }

private:
	unsigned int m_MatchId {0};
	MatchScore m_Score;
	std::string m_BettorName;
};

