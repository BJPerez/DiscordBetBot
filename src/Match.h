#pragma once

#include <optional>
#include <string>

#include "MatchScore.h"
#include "gtest/gtest_prod.h"

class Match
{
public:
	static constexpr unsigned int FIRST_ID = 0;
	static constexpr std::string_view ID_PREFIX = "MATCH_BOT_ID_";
	static constexpr std::string_view INVALID_ID = "MATCH_INVALID_ID";
	static constexpr unsigned int MINIMAL_BO_SIZE = 1;

	Match() = default; // Should only be used by serialization
	Match(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int maxNumberOfGame);

	bool operator==(const Match& match) const noexcept { return m_Id == match.m_Id; }

	[[nodiscard]] const std::string& GetId() const noexcept { return m_Id; }
	[[nodiscard]] const std::string& GetTeamAName() const noexcept { return m_TeamAName; }
	[[nodiscard]] const std::string& GetTeamBName() const noexcept { return m_TeamBName; }
	[[nodiscard]] unsigned int GetBoSize() const noexcept { return m_BoSize; }
	[[nodiscard]] unsigned int GetNumberOfGamesToWin() const noexcept { return (GetBoSize() / 2) + 1; }
	[[nodiscard]] bool IsPlayed() const noexcept { return m_Result.has_value(); }
	[[nodiscard]] bool IsValidScore(const MatchScore& score) const noexcept;
	[[nodiscard]] const MatchScore& GetResult() const;

	void SetResult(const MatchScore& score);

	// All these setters should only be called by serialization when loading the save file
	// The loading should happen before any new creation of match.
	// Because if we have no way to know which IDs are already used before we load.
	void SetId(const std::string& id);
	void SetTeamAName(std::string name) noexcept { m_TeamAName = std::move(name); }
	void SetTeamBName(std::string name) noexcept { m_TeamBName = std::move(name); }
	void SetBoSize(const unsigned int size) noexcept { m_BoSize = size; }

	[[nodiscard]] std::string ToString() const noexcept { return m_TeamAName + " - " + m_TeamBName; }

private:
	inline static unsigned int s_NextId{ FIRST_ID };

	std::string m_Id {INVALID_ID};
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize {0}; // Max number of games for the match, must be an odd number.
	std::optional<MatchScore> m_Result {std::nullopt};

#pragma region UnitTestSpecificFriend
	FRIEND_TEST(Match_Tests, SetIdWithCustom);
	FRIEND_TEST(Match_Tests, SetIdWithoutCustom);
#pragma endregion 
};

