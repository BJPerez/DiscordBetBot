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
	virtual ~ICommandReceiver();

	virtual void AddMatch(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int boSize) = 0;
	virtual void AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName) = 0;
	virtual void AddResult(const std::string& matchId, const MatchScore& matchResult) = 0;
	virtual void ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName) = 0;

	[[nodiscard]] virtual std::optional<std::reference_wrapper<const Bet>> GetBet(const std::string& matchId, const std::string& bettorName) const = 0;
	[[nodiscard]] virtual std::optional<std::reference_wrapper<const Match>> GetMatch(const std::string& matchId) const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<const Bet>> GetBetsOnMatch(const std::string& matchId) const = 0;
	[[nodiscard]] virtual const std::vector<Match>& GetIncomingMatches() const = 0;
	[[nodiscard]] virtual const std::vector<Bet>& GetBets() const = 0;
	[[nodiscard]] virtual const std::vector<BettorResults>& GetBettorsResults() const = 0;

	virtual void SetIncomingMatches(std::vector<Match> matches) = 0;
	virtual void SetBets(std::vector<Bet> bets) = 0;
	virtual void SetBettorResults(std::vector<BettorResults> results) = 0;
};

