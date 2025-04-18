#pragma once

#include <optional>
#include <string>
#include <vector>

class BettorResults;
class Bet;
struct MatchScore;
class Match;

class ICommandReceiver  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
	virtual ~ICommandReceiver() = default;

	virtual void AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName) = 0;
	virtual void ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName) = 0;

	struct AddMatchParams
	{
		std::optional<std::string> m_MatchId;
		std::string m_TeamAName;
		std::string m_TeamBName;
		unsigned int m_BoSize;
		std::string m_TimeAsString;
	};
	virtual void AddMatch(const AddMatchParams& params) = 0;

	struct AddResultParams
	{
		std::string m_MatchId;
		std::optional<std::string> m_TeamAName;
		std::optional<std::string> m_TeamBName;
		unsigned int m_TeamAScore;
		unsigned int m_TeamBScore;
	};
	virtual void AddResult(const AddResultParams& params) = 0;

	[[nodiscard]] virtual const Bet& GetBet(const std::string& matchId, const std::string& bettorName) const = 0;
	[[nodiscard]] virtual const Match& GetMatch(const std::string& matchId) const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<const Bet>> GetBetsOnMatch(const std::string& matchId) const = 0;
	[[nodiscard]] virtual const std::vector<Match>& GetAllMatches() const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<const Match>> GetIncomingMatches() const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<const Match>> GetPastMatches() const = 0;
	[[nodiscard]] virtual const std::vector<Bet>& GetBets() const noexcept = 0;

	virtual void SetMatches(std::vector<Match> matches) = 0;
	virtual void SetBets(std::vector<Bet> bets) = 0;

	[[nodiscard]] virtual bool HasMatch(const std::string& matchId) const = 0;
	[[nodiscard]] virtual bool HasBet(const std::string& matchId, const std::string& bettorName) const = 0;
};

