#pragma once

#include <string>

struct MatchScore
{
	enum class EComparingResult
	{
		Perfect, // The two scores have the same winning team and the same scores for team A and B
		Correct, // The two scores have the same winning team but not the same scores for the loosing team
		Incorrect, // The two scores don't have the same winning team
		Invalid // One or both the scores don't have a winner or the two scores are not compatible. For example 2-1 which is BO3 with 3-0 which is BO5
	};

	unsigned int m_TeamAScore{ 0 };
	unsigned int m_TeamBScore{ 0 };

	[[nodiscard]] bool operator==(const MatchScore& result) const noexcept { return Compare(result) == EComparingResult::Perfect; }
	[[nodiscard]] EComparingResult Compare(const MatchScore& other) const noexcept;

	[[nodiscard]] std::string ToString() const { return std::to_string(m_TeamAScore) + " - " + std::to_string(m_TeamBScore); }

	[[nodiscard]] unsigned int GetTotalNumberOfGames() const noexcept { return m_TeamAScore + m_TeamBScore; }

	[[nodiscard]] unsigned int GetLoosingTeamScore() const noexcept { return m_TeamAScore > m_TeamBScore ? m_TeamBScore : m_TeamAScore; }
	[[nodiscard]] unsigned int GetWinningTeamScore() const noexcept { return m_TeamAScore > m_TeamBScore ? m_TeamAScore : m_TeamBScore; }
};

