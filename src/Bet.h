#pragma once

#include <string>

#include "MatchScore.h"

class Bet
{
public:
	Bet() = default; // should only be used by serialization
	Bet(std::string matchId, const MatchScore& score, std::string bettorName) noexcept :
		m_MatchId(std::move(matchId)), m_Score{ score }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }
	[[nodiscard]] const MatchScore& GetScore() const noexcept { return m_Score; }
	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; } 

	void SetMatchId(const std::string& id) noexcept { m_MatchId = id; }
	void SetScore(const MatchScore& score) noexcept { m_Score = score; }
	void SetBettorName(std::string bettorName) noexcept { m_BettorName = std::move(bettorName); }

private:
	std::string m_MatchId;
	MatchScore m_Score;
	std::string m_BettorName;
};

