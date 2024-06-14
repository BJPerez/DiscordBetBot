#pragma once

#include <dpp/dpp.h>

#include <shared_mutex>

#include "BotData.h"
#include "ICommandReceiver.h"
#include "JsonSerializer.h"
#include "LockableDataAccessors.h"
#include "SaveManager.h"

class BetBot
{
public:
	BetBot(const std::string& betToken, std::string saveFilePath);

	void Start();
	[[nodiscard]] DataReader<ICommandReceiver> GetDataReader() const { return { m_Data, m_DataMutex }; };
	[[nodiscard]] DataWriter<ICommandReceiver> GetDataWriter() { return { m_Data, m_DataMutex }; };

private:
	dpp::cluster m_Cluster;
	BotData m_Data;
	mutable std::shared_mutex m_DataMutex;
	SaveManager m_Saver;

	void CreateCommands();
	void SetUpCallbacks();
	void SetUpCommandCallbacks();
	void SetUpSelectCallbacks();

	[[nodiscard]] dpp::slashcommand CreateAddMatchCommand() const;
	[[nodiscard]] dpp::slashcommand CreateShowMatchesCommand() const { return { "show_matches", "Show the incoming matches. Also allows you to bet on them or set their result.", m_Cluster.me.id }; }
	[[nodiscard]] dpp::slashcommand CreateShowResultsCommand() const { return { "show_results", "Show the different bettors and their results.", m_Cluster.me.id }; }

	void ExecuteAddMatch(const dpp::slashcommand_t& event);
	void ExecuteShowMatches(const dpp::slashcommand_t& event);
	void ExecuteShowResults(const dpp::slashcommand_t& event);
	void ExecuteShowBetProposal(const dpp::select_click_t& event);
	void ExecuteAddBet(const dpp::select_click_t& event);
	void ExecuteChooseMatchToBetOn(const dpp::select_click_t& event);
	void ExecuteChooseMatchToSetResult(const dpp::select_click_t& event);
	void ExecuteAddResult(const dpp::select_click_t& event);
	void ExecuteShowResultProposal(const dpp::select_click_t& event);
};