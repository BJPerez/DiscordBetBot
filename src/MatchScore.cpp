#include "MatchScore.h"

MatchScore::ComparingResult MatchScore::Compare(const MatchScore& other) const noexcept
{
	if (m_TeamAScore == other.m_TeamAScore && m_TeamBScore == other.m_TeamBScore)
	{
		return ComparingResult::Perfect;
	}
	else if
		( 
			( m_TeamAScore > m_TeamBScore && other.m_TeamAScore > other.m_TeamBScore ) ||
			( m_TeamBScore > m_TeamAScore && other.m_TeamBScore > other.m_TeamAScore ) 
		)
	{
		return ComparingResult::Correct;
	}
	return ComparingResult::Incorrect;
}