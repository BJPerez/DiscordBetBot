#include "MatchScore.h"

MatchScore::EComparingResult MatchScore::Compare(const MatchScore& other) const noexcept
{
	if (GetWinningTeamScore() != other.GetWinningTeamScore()) // Tells us if both score has the same BoSize
	{
		return EComparingResult::Invalid;
	}

	if (m_TeamAScore == other.m_TeamAScore && m_TeamBScore == other.m_TeamBScore)
	{
		return EComparingResult::Perfect;
	}

	if
		( 
			( m_TeamAScore > m_TeamBScore && other.m_TeamAScore > other.m_TeamBScore ) ||
			( m_TeamBScore > m_TeamAScore && other.m_TeamBScore > other.m_TeamAScore ) 
		)
	{
		return EComparingResult::Correct;
	}

	return EComparingResult::Incorrect;
}