#include "Match.h"

Match::Match(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame) :
	m_TeamAName(std::move(teamAName)),
	m_TeamBName(std::move(teamBName)),
	m_BoSize(maxNumberOfGame)
{
	if (matchId.has_value())
	{
		m_Id = std::move(matchId.value());
	}
	else
	{
		m_Id = std::string(ID_PREFIX) + std::to_string(s_NextId++);
	}
}

void Match::SetId(std::string id)
{
	m_Id = std::move(id);

	// We update s_NextId only if the given id is a custom id
	// We do not want to do it if it is an id generated outside of the bot 
	if (const std::string idPrefix = m_Id.substr(0, m_Id.find_last_of('_')); 
		idPrefix == std::string(ID_PREFIX))
	{
		if (const unsigned int idValue = std::stol(m_Id.substr(m_Id.find_last_of('_')));
			s_NextId <= idValue)
		{
			s_NextId = idValue + 1;
		}
	}
}