#pragma once

struct MatchScore
{
	unsigned int m_TeamAScore{ 0 };
	unsigned int m_TeamBScore{ 0 };

	bool operator==(const MatchScore& result) const
	{
		return m_TeamAScore == result.m_TeamAScore && m_TeamBScore == result.m_TeamBScore;
	}

	[[nodiscard]] std::string ToString() const { return std::to_string(m_TeamAScore) + " - " + std::to_string(m_TeamBScore); }

	[[nodiscard]] unsigned int GetTotalNumberOfGames() const { return m_TeamAScore + m_TeamBScore; }
};

