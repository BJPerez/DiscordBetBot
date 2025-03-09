#include "Match.h"

#include "BotDataExceptions.h"

Match::Match(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame, 
	const DateAndTime& dateTime) :
		m_TeamAName(std::move(teamAName)),
		m_TeamBName(std::move(teamBName)),
		m_BoSize(maxNumberOfGame),
		m_DateAndTime(dateTime)
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

void Match::SetId(const std::string& id)
{
	m_Id = id;

	// We update s_NextId only if the given id is a custom id
	// We do not want to do it if it is an id generated outside of the bot 
	if (const std::string idPrefix = m_Id.substr(0, m_Id.find_last_of('_')+1); // we want the "_" found in idPrefix thus +1
		idPrefix == std::string(ID_PREFIX))
	{
		const std::string idAsString = m_Id.substr(m_Id.find_last_of('_')+1);
		if (const unsigned int idValue = std::stol(idAsString);
			s_NextId <= idValue)
		{
			s_NextId = idValue + 1;
		}
	}
}

bool Match::IsValidScore(const MatchScore& score) const noexcept
{
	if (m_BoSize < score.GetTotalNumberOfGames())
	{
		return false;
	}

	if (const unsigned int numberOfGamesToWin = GetNumberOfGamesToWin();
		score.m_TeamAScore != numberOfGamesToWin && score.m_TeamBScore != numberOfGamesToWin)
	{
		return false;
	}

	return true;
}

const MatchScore& Match::GetResult() const
{
	if (!m_Result.has_value())
	{
		throw MatchNotPlayedException(m_Id);
	}

	return m_Result.value();
}

void Match::SetResult(const MatchScore& score) 
{
	if (!IsValidScore(score))
	{
		throw InvalidScoreException(m_BoSize, score);
	}

	m_Result = score;
};