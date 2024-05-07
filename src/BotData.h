#pragma once

#include <vector>

#include "Bet.h"
#include "BettorResults.h"
#include "ICommandReceiver.h"
#include "Match.h"

class BotData final : public ICommandReceiver
{
public:
	void AddMatch(std::string teamAName, std::string teamBName, const unsigned int boSize) final;
	void AddBet(const unsigned int matchId, const MatchScore& matchResult, std::string bettorName) final;
	void AddResult(const unsigned int matchId, const MatchScore& matchResult) final;
	void ModifyBet(const unsigned int matchId, const MatchScore& matchResult, const std::string& bettorName) final;

	[[nodiscard]] std::optional<std::reference_wrapper<const Match>> GetMatch(const unsigned int matchId) const final;
	[[nodiscard]] std::optional<std::reference_wrapper<const Bet>> GetBet(const unsigned int matchId, const std::string& bettorName) const final;
	[[nodiscard]] const std::vector<Bet>& GetBets() const final { return m_Bets; }
	[[nodiscard]] const std::vector<Match>& GetIncomingMatches() const final { return m_IncomingMatches; }
	[[nodiscard]] const std::vector<BettorResults>& GetBettorsResults() const final { return m_BettorResults; }

	void SetIncomingMatches(std::vector<Match> matches) { m_IncomingMatches = std::move(matches); }
	void SetBets(std::vector<Bet> bets) { m_Bets = std::move(bets); }
	void SetBettorResults(std::vector<BettorResults> results);

private:
	std::vector<Match> m_IncomingMatches;
	std::vector<Bet> m_Bets;
	std::vector<BettorResults> m_BettorResults;

	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsLinkedToMatch(const unsigned int matchId) const;
	[[nodiscard]] std::optional<std::reference_wrapper<Bet>> GetBet(const unsigned int matchId, const std::string& bettorName);

	[[nodiscard]] BettorResults& GetOrCreateBettorResults(std::string bettorName);


	void EraseBetsLinkedToMatch(const unsigned int matchId);
	void SortResults();
};

