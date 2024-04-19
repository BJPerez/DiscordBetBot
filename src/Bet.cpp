#include "Bet.h"

Bet::Result Bet::EvaluateBet(const MatchScore& matchScore) const
{
	if (m_Score == matchScore)
	{
		return Result::PerfectBet;
	}
	else if (
		(m_Score.m_TeamAScore > m_Score.m_TeamBScore && matchScore.m_TeamAScore > matchScore.m_TeamBScore) ||
		(m_Score.m_TeamBScore > m_Score.m_TeamAScore && matchScore.m_TeamBScore > matchScore.m_TeamAScore)
		)
	{
		return Result::CorrectBet;
	}

	return Result::IncorrectBet;
}
