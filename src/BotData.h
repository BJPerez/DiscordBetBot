#pragma once

#include <functional>
#include <vector>

#include "Bet.h"
#include "BettorResults.h"
#include "ICommandReceiver.h"
#include "Match.h"

class BotData final : public ICommandReceiver
{
public:
	void AddMatch(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int boSize) override;
	void AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName) override;
	void AddResult(const std::string& matchId, const MatchScore& matchResult) override;
	void ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName) override;

	[[nodiscard]] std::optional<std::reference_wrapper<const Match>> GetMatch(const std::string& matchId) const override;
	[[nodiscard]] std::optional<std::reference_wrapper<const Bet>> GetBet(const std::string& matchId, const std::string& bettorName) const override;
	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsOnMatch(const std::string& matchId) const override;
	[[nodiscard]] const std::vector<Bet>& GetBets() const override { return m_Bets; }
	[[nodiscard]] const std::vector<Match>& GetIncomingMatches() const override { return m_IncomingMatches; }
	[[nodiscard]] const std::vector<BettorResults>& GetBettorsResults() const override { return m_BettorResults; }

	void SetIncomingMatches(std::vector<Match> matches) override  { m_IncomingMatches = std::move(matches); }
	void SetBets(std::vector<Bet> bets) override  { m_Bets = std::move(bets); }
	void SetBettorResults(std::vector<BettorResults> results) override;

private:
	std::vector<Match> m_IncomingMatches;
	std::vector<Bet> m_Bets;
	std::vector<BettorResults> m_BettorResults;

	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsLinkedToMatch(const std::string& matchId) const;
	[[nodiscard]] std::optional<std::reference_wrapper<Bet>> GetBet(const std::string& matchId, const std::string& bettorName);

	[[nodiscard]] BettorResults& GetOrCreateBettorResults(std::string bettorName);


	void EraseBetsLinkedToMatch(const std::string& matchId);

	using BetFilter = std::function<bool(const Bet& bet)>;
	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsWithFilter(const BetFilter& filter) const;
	[[nodiscard]] std::vector<std::reference_wrapper<Bet>> GetBetsWithFilter(const BetFilter& filter);
};

