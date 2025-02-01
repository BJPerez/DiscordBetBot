#pragma once

#include <functional>
#include <optional>
#include <string>
#include <vector>

#include "Bet.h"
#include "ICommandReceiver.h"
#include "Match.h"
#include "MatchScore.h"

template<typename Filter>
concept BetFilter = requires(Filter filter, const Bet& bet)
{
	{ filter(bet) } -> std::convertible_to<bool>;
};

template<typename Filter>
concept MatchFilter = requires(Filter filter, const Match& match)
{
	{ filter(match) } -> std::convertible_to<bool>;
};

class BotData final : public ICommandReceiver
{
public:
	void AddMatch(std::optional<std::string> matchId, std::string teamAName, std::string teamBName, const unsigned int boSize) override;
	void AddBet(std::string matchId, const MatchScore& matchResult, std::string bettorName) override;
	void AddResult(const std::string& matchId, const MatchScore& matchResult) override;
	void ModifyBet(const std::string& matchId, const MatchScore& matchResult, const std::string& bettorName) override;

	[[nodiscard]] const Match& GetMatch(const std::string& matchId) const override;
	[[nodiscard]] const Bet& GetBet(const std::string& matchId, const std::string& bettorName) const override;
	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsOnMatch(const std::string& matchId) const override;
	[[nodiscard]] const std::vector<Bet>& GetBets() const noexcept override { return m_Bets; }
	[[nodiscard]] const std::vector<Match>& GetAllMatches() const override { return m_Matches; }
	[[nodiscard]] std::vector<std::reference_wrapper<const Match>> GetIncomingMatches() const override;
	[[nodiscard]] std::vector<std::reference_wrapper<const Match>> GetPastMatches() const override;

	void SetMatches(std::vector<Match> matches) override  { m_Matches = std::move(matches); }
	void SetBets(std::vector<Bet> bets) override  { m_Bets = std::move(bets); }

	[[nodiscard]] bool HasMatch(const std::string& matchId) const override;
	[[nodiscard]] bool HasBet(const std::string& matchId, const std::string& bettorName) const override;

private:
	std::vector<Match> m_Matches;
	std::vector<Bet> m_Bets;

	[[nodiscard]] Bet& GetBet(const std::string& matchId, const std::string& bettorName);
	[[nodiscard]] Match& GetMatch(const std::string& matchId);

	template<BetFilter Filter>
	[[nodiscard]] std::vector<std::reference_wrapper<Bet>> GetBetsWithFilter(Filter filter)
	{
		std::vector<std::reference_wrapper<Bet>> bets;

		for (Bet& bet : m_Bets)
		{
			if (filter(bet))
			{
				bets.emplace_back(bet);
			}
		}

		return bets;
	}

	template<BetFilter Filter>
	[[nodiscard]] std::vector<std::reference_wrapper<const Bet>> GetBetsWithFilter(Filter filter) const
	{
		std::vector<std::reference_wrapper<const Bet>> bets;

		for (const Bet& bet : m_Bets)
		{
			if (filter(bet))
			{
				bets.emplace_back(bet);
			}
		}

		return bets;
	}

	template<MatchFilter Filter>
	[[nodiscard]] std::vector<std::reference_wrapper<Match>> GetMatchesWithFilter(Filter filter)
	{
		std::vector<std::reference_wrapper<Match>> matches;

		for (Match& match : m_Matches)
		{
			if (filter(match))
			{
				matches.emplace_back(match);
			}
		}

		return matches;
	}

	template<MatchFilter Filter>
	[[nodiscard]] std::vector<std::reference_wrapper<const Match>> GetMatchesWithFilter(Filter filter) const
	{
		std::vector<std::reference_wrapper<const Match>> matches;

		for (const Match& match : m_Matches)
		{
			if (filter(match))
			{
				matches.emplace_back(match);
			}
		}

		return matches;
	}
};

