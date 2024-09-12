#pragma once

#include <dpp/dpp.h>

#include <shared_mutex>

#include "BotData.h"
#include "FileWatcher.h"
#include "ICommandReceiver.h"
#include "JsonSerializer.h"
#include "LockableDataAccessors.h"
#include "SaveManager.h"

namespace BotConfigReader
{
	struct Results;
}

class BetBot
{
public:
	explicit BetBot(BotConfigReader::Results config);

	void Start();
	[[nodiscard]] DataReader<ICommandReceiver> GetDataReader() const { return { m_Data, m_DataMutex }; };
	[[nodiscard]] DataWriter<ICommandReceiver> GetDataWriter() { return { m_Data, m_DataMutex }; };

private:
	dpp::cluster m_Cluster;
	dpp::snowflake m_AnswerChannelId;
	BotData m_Data;
	mutable std::shared_mutex m_DataMutex;
	SaveManager m_Saver;
	std::optional<FileWatcher> m_NewMatchWatcher;
	std::optional<FileWatcher> m_NewResultWatcher;

	void CreateCommands();
	void SetUpCallbacks();
	void SetUpCommandCallbacks();
	void SetUpSelectCallbacks();
	void SetupFileWatchers(BotConfigReader::Results& config);

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
	void OnNewMatch(const std::filesystem::path& file);
	void OnNewResult(const std::filesystem::path& file);
};
