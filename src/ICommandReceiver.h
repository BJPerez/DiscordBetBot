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

	virtual void AddMatch(std::string teamAName, std::string teamBName, const unsigned int boSize) = 0;
	virtual void AddBet(const unsigned int matchId, const MatchScore& matchResult, std::string bettorName) = 0;
	virtual void AddResult(const unsigned int matchId, const MatchScore& matchResult) = 0;
	virtual void ModifyBet(const unsigned int matchId, const MatchScore& matchResult, const std::string& bettorName) = 0;

	[[nodiscard]] virtual std::optional<std::reference_wrapper<const Bet>> GetBet(const unsigned int matchId, const std::string& bettorName) const = 0;
	[[nodiscard]] virtual std::optional<std::reference_wrapper<const Match>> GetMatch(const unsigned int matchId) const = 0;
	[[nodiscard]] virtual std::vector<std::reference_wrapper<const Bet>> GetBetsOnMatch(const unsigned int matchId) const = 0;
	[[nodiscard]] virtual const std::vector<Match>& GetIncomingMatches() const = 0;
	[[nodiscard]] virtual const std::vector<Bet>& GetBets() const = 0;
	[[nodiscard]] virtual const std::vector<BettorResults>& GetBettorsResults() const = 0;
};

