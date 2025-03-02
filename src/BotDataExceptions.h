#pragma once

#include <exception>

#include "MatchScore.h"

class InvalidBettorNameException final : public std::exception
{
public:
	explicit InvalidBettorNameException(std::string name) noexcept : m_BettorName(std::move(name)) {}

	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; }

private:
	std::string m_BettorName;
};

class MatchNotFoundException final : public std::exception
{
public:
	explicit MatchNotFoundException(std::string id) noexcept : m_MatchId(std::move(id)){}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
};

class MatchAlreadyPlayedException final : public std::exception
{
public:
	explicit MatchAlreadyPlayedException(std::string id) noexcept : m_MatchId(std::move(id)) {}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
};

class InvalidScoreException final : public std::exception
{
public:
	explicit InvalidScoreException(const unsigned int boSize, const MatchScore score) noexcept : m_BoSize(boSize), m_Score(score) {}

	[[nodiscard]] unsigned int GetBoSize() const noexcept { return m_BoSize; }
	[[nodiscard]] MatchScore GetScore() const noexcept { return m_Score; }

private:
	unsigned int m_BoSize;
	MatchScore m_Score;
};

class BetAlreadyExistException final : public std::exception
{
public:
	explicit BetAlreadyExistException(std::string matchId, std::string name) noexcept : m_MatchId(std::move(matchId)), m_BettorName(std::move(name)) {}

	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; }
	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
	std::string m_BettorName;
};

class BetNotFoundException final : public std::exception
{
public:
	explicit BetNotFoundException(std::string matchId, std::string name) noexcept : m_MatchId(std::move(matchId)), m_BettorName(std::move(name)) {}

	[[nodiscard]] const std::string& GetBettorName() const noexcept { return m_BettorName; }
	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
	std::string m_BettorName;
};

class InvalidMatchIdException final : public std::exception
{
public:
	explicit InvalidMatchIdException(std::string id) noexcept : m_MatchId(std::move(id)) {}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
};

class MatchIdUnavailableException final : public std::exception
{
public:
	explicit MatchIdUnavailableException(std::string id) noexcept : m_MatchId(std::move(id)) {}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
};

class InvalidTeamNameException final : public std::exception
{
public:
	explicit InvalidTeamNameException(std::string teamA, std::string teamB) noexcept : m_TeamAName(std::move(teamA)), m_TeamBName(std::move(teamB)) {}

	[[nodiscard]] const std::string& GetTeamAName() const noexcept { return m_TeamAName; }
	[[nodiscard]] const std::string& GetTeamBName() const noexcept { return m_TeamBName; }

private:
	std::string m_TeamAName;
	std::string m_TeamBName;
};

class InvalidBoSizeException final : public std::exception
{
public:
	explicit InvalidBoSizeException(const unsigned int boSize) noexcept : m_BoSize(boSize) {}

	[[nodiscard]] unsigned int GetBoSize() const noexcept { return m_BoSize; }

private:
	unsigned int m_BoSize;
};

class MatchNotPlayedException final : public std::exception
{
public:
	explicit MatchNotPlayedException(std::string id) noexcept : m_MatchId(std::move(id)) {}

	[[nodiscard]] const std::string& GetMatchId() const noexcept { return m_MatchId; }

private:
	std::string m_MatchId;
};

class IndexInvalidException final : public std::exception
{
public:
	explicit IndexInvalidException(const size_t index, const size_t arraySize) noexcept : m_Index(index),
		m_ArraySize(arraySize) {}

	[[nodiscard]] size_t GetIndex() const noexcept { return m_Index; }
	[[nodiscard]] size_t GetArraySize() const noexcept { return m_ArraySize; }

private:
	size_t m_Index;
	size_t m_ArraySize;
};

class UnmatchingTeamNameException final : std::exception
{
public:
	explicit UnmatchingTeamNameException(std::string givenA, std::string givenB, std::string savedA, std::string savedB) noexcept :
		m_GivenAName(std::move(givenA)), m_GivenBName(std::move(givenB)), m_SavedAName(std::move(savedA)),
		m_SavedBName(std::move(savedB)) {}

	[[nodiscard]] const std::string& GetGivenAName() const noexcept { return m_GivenAName; }
	[[nodiscard]] const std::string& GetGivenBName() const noexcept { return m_GivenBName; }
	[[nodiscard]] const std::string& GetSavedAName() const noexcept { return m_SavedAName; }
	[[nodiscard]] const std::string& GetSavedBName() const noexcept { return m_SavedBName; }

private:
	std::string m_GivenAName;
	std::string m_GivenBName;

	std::string m_SavedAName;
	std::string m_SavedBName;
};

