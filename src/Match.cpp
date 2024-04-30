#include "Match.h"

Match::Match(std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame) :
	m_TeamAName(std::move(teamAName)), m_TeamBName(std::move(teamBName)), m_BoSize(maxNumberOfGame)
{
	m_Id = s_NextId++;
}

void Match::SetId(const unsigned int id)
{
	m_Id = id;

	if (s_NextId <= m_Id)
	{
		s_NextId = m_Id + 1;
	}
}