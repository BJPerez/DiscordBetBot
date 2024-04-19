#pragma once

#include <string>

class Match
{
public:
	static constexpr unsigned int FIRST_ID = 0;
	static constexpr unsigned int INVALID_ID = static_cast<unsigned int>(-1);
	static constexpr unsigned int MINIMAL_BO_SIZE = 1;

	Match(std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame) :
		m_TeamAName(std::move(teamAName)), m_TeamBName(std::move(teamBName)), m_BoSize(maxNumberOfGame)
	{
		m_Id = s_NextId++;
	}

	bool operator==(const Match& match) const { return m_Id == match.m_Id; }

	[[nodiscard]] unsigned int GetId() const { return m_Id; }
	[[nodiscard]] const std::string& GetTeamAName() const { return m_TeamAName; }
	[[nodiscard]] const std::string& GetTeamBName() const { return m_TeamBName; }
	[[nodiscard]] unsigned int GetBoSize() const { return m_BoSize; }
	[[nodiscard]] unsigned int GetNumberOfGamesToWin() const { return (GetBoSize() / 2) + 1; }

private:
	inline static unsigned int s_NextId{ FIRST_ID };

	unsigned int m_Id {0};
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize {0}; // Max number of games for the match, must be an odd number.
};

