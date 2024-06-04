#pragma once

#include <string>

#include "MatchScore.h"

class Bet
{
public:
	Bet() = default; // should only be used by serialization
	Bet(const unsigned int matchId, const MatchScore& score, std::string bettorName) noexcept :
		m_MatchId(matchId), m_Score{ score }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] unsigned int GetMatchId() const noexcept { return m_MatchId; }
	[[nodiscard]] const MatchScore& GetScore() const noexcept { return m_Score; }
	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; }

	void SetMatchId(const unsigned int id) noexcept { m_MatchId = id; }
	void SetScore(const MatchScore& score) noexcept { m_Score = score; }
	void SetBettorName(std::string bettorName) noexcept { m_BettorName = std::move(bettorName); }

private:
	unsigned int m_MatchId {0};
	MatchScore m_Score;
	std::string m_BettorName;
};

