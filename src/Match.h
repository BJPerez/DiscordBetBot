#pragma once

#include <string>

class Match
{
public:
	static constexpr unsigned int FIRST_ID = 0;
	static constexpr unsigned int INVALID_ID = static_cast<unsigned int>(-1);
	static constexpr unsigned int MINIMAL_BO_SIZE = 1;

	Match() = default; // Should only be used by serialization
	Match(std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame);

	bool operator==(const Match& match) const { return m_Id == match.m_Id; }

	[[nodiscard]] unsigned int GetId() const { return m_Id; }
	[[nodiscard]] const std::string& GetTeamAName() const { return m_TeamAName; }
	[[nodiscard]] const std::string& GetTeamBName() const { return m_TeamBName; }
	[[nodiscard]] unsigned int GetBoSize() const { return m_BoSize; }
	[[nodiscard]] unsigned int GetNumberOfGamesToWin() const { return (GetBoSize() / 2) + 1; }

	// SetID should only be used by serialization when loading the save file. And the loading should happen before any new creation of match.
	// Because if we have no way to know which IDs are already used before we load. 
	void SetId(const unsigned int id);
	void SetTeamAName(std::string name) { m_TeamAName = std::move(name); }
	void SetTeamBName(std::string name) { m_TeamBName = std::move(name); }
	void SetBoSize(const unsigned int size) { m_BoSize = size; }

private:
	inline static unsigned int s_NextId{ FIRST_ID };

	unsigned int m_Id {0};
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize {0}; // Max number of games for the match, must be an odd number.
};

