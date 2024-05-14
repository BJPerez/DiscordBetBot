#pragma once

#include <dpp/dpp.h>

#include "BotData.h"
#include "JsonSerializer.h"
#include "SaveManager.h"

class BetBot
{
public:
	BetBot(const std::string& betToken, std::string saveFilePath);

	void Start();

private:
	dpp::cluster m_Cluster;
	BotData m_Data;
	SaveManager m_Saver;

	void CreateCommands();
	void SetUpCallbacks();

	dpp::slashcommand CreateAddMatchCommand() const;
	dpp::slashcommand CreateAddBetCommand() const;
	dpp::slashcommand CreateAddResultCommand() const;
	dpp::slashcommand CreateShowMatchesCommand() const { return { "show_matches", "Show the incoming matches on which you can bet.", m_Cluster.me.id }; }
	dpp::slashcommand CreateShowBetsCommand() const { return { "show_bets", "Show the different ongoing bets.", m_Cluster.me.id }; }
	dpp::slashcommand CreateShowResultsCommand() const { return { "show_results", "Show the different bettors and their results.", m_Cluster.me.id }; }

	void ExecuteAddMatch(const dpp::slashcommand_t& event);
	void ExecuteAddResult(const dpp::slashcommand_t& event);
	void ExecuteShowMatches(const dpp::slashcommand_t& event);
	void ExecuteShowBets(const dpp::slashcommand_t& event);
	void ExecuteShowResults(const dpp::slashcommand_t& event);
	void ExecuteShowBetProposal(const dpp::select_click_t& event);
	void ExecuteAddBet(const dpp::select_click_t& event);
};