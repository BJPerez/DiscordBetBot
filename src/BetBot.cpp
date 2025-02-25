#include "BetBot.h"

#include "AddBetCommand.h"
#include "AddMatchCommand.h"
#include "AddResultCommand.h"
#include "BotConfigReader.h"
#include "ChooseMatchToBetOnCommand.h"
#include "ChooseMatchToSetResultCommand.h"
#include "ShowBetProposalCommand.h"
#include "ShowBettorsResultsCommand.h"
#include "ShowIncomingMatchesCommand.h"
#include "ShowResultProposalCommand.h"

namespace
{
	void ExtractMatchInfosFromSelectorValue(std::string selectorValue, std::string& outMatchId, unsigned int& outTeamAScore, unsigned int& outTeamBScore)
	{
		// format of the value is "MatchId-AScore-BScore"

		std::size_t delimiter = selectorValue.find('-');
		outMatchId = selectorValue.substr(0, delimiter);
		selectorValue = selectorValue.substr(delimiter + 1);

		delimiter = selectorValue.find('-');
		outTeamAScore = std::stoul(selectorValue.substr(0, delimiter));
		selectorValue = selectorValue.substr(delimiter + 1);

		outTeamBScore = std::stoul(selectorValue);
	}
}

BetBot::BetBot(BotConfigReader::Results config) : m_Cluster(std::move(config.m_BotToken)), m_AnswerChannelId(config.m_AnswerChannelId), m_Data(),
	m_Saver(std::move(config.m_SaveFilePath))
{
	m_Saver.Load(GetDataWriter());
	m_Cluster.on_log(dpp::utility::cout_logger());

	CreateCommands();
	SetUpCallbacks();
	SetupFileWatchers(config);

	srand(time(nullptr)); // Init randomness
}

void BetBot::SetupFileWatchers(BotConfigReader::Results& config)
{
	if (config.m_NewMatchesFolder.has_value() && config.m_NewResultsFolder.has_value() && config.m_DelayBetweenChecks.has_value())
	{
		m_NewMatchWatcher.emplace(std::move(config.m_NewMatchesFolder.value()), config.m_DelayBetweenChecks.value(),
			[this](const std::filesystem::path& filePath)
			{
				OnNewMatch(filePath);
			}
		);

		m_NewResultWatcher.emplace(std::move(config.m_NewResultsFolder.value()), std::move(config.m_DelayBetweenChecks.value()),
			[this](const std::filesystem::path& filePath)
			{
				OnNewResult(filePath);
			}
		);
	}
}

dpp::slashcommand BetBot::CreateAddMatchCommand() const
{
	dpp::slashcommand command("add_match", "Add an incoming match.", m_Cluster.me.id);
	command.add_option(dpp::command_option(dpp::co_string, "team_a", "The name of the first team.", true));
	command.add_option(dpp::command_option(dpp::co_string, "team_b", "The name of the second team.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "bo_size", "The maximal number of game of this match.", true));
	command.add_option(dpp::command_option(dpp::co_string, "date_time", 
		"The date and time of the match with this format: Day-Month-Year Hours:Minutes>", true));

	return command;
}

void BetBot::CreateCommands()
{
	m_Cluster.on_ready(
		[this](const dpp::ready_t&)
		{
			/* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
			if (dpp::run_once<struct register_bot_commands>())
			{
				std::vector<dpp::slashcommand> commands;
				commands.emplace_back(CreateAddMatchCommand());
				commands.emplace_back(CreateShowMatchesCommand());
				commands.emplace_back(CreateShowResultsCommand());

				m_Cluster.global_bulk_command_create(commands);
			}
		}
	);
}

void BetBot::ExecuteAddMatch(const dpp::slashcommand_t& event)
{
	auto teamAName = std::get<std::string>(event.get_parameter("team_a"));
	auto teamBName = std::get<std::string>(event.get_parameter("team_b"));
	const unsigned int boSize = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("bo_size")));
	auto dateTimeAsString = std::get<std::string>(event.get_parameter("date_time"));

	const AddMatchCommand command{ m_AnswerChannelId, *this, std::move(teamAName), std::move(teamBName), boSize, dateTimeAsString };
	event.reply(command.Execute());
	m_Saver.Save(GetDataReader());
}

void BetBot::OnNewMatch(const std::filesystem::path& file)
{
	if (std::ifstream fileStream{ file.relative_path() };
		fileStream.good()) // does file exists ?
	{
		dpp::json fileContent;
		fileStream >> fileContent;
		// TODO
		if (fileContent.contains("matchid") &&
			fileContent.contains("team1") &&
			fileContent.contains("team2") &&
			fileContent.contains("bo_size") 
			)
		{
			const std::string boSizeStr = fileContent["bo_size"];
			const unsigned int boSize = std::stol(boSizeStr);

			const AddMatchCommand command{ m_AnswerChannelId, *this, fileContent["team1"], fileContent["team2"], boSize, fileContent["matchid"] };
			command.Execute(); 

			m_Saver.Save(GetDataReader());

			fileStream.close();
		}

		std::filesystem::remove(file);
	}
}

void BetBot::ExecuteAddBet(const dpp::select_click_t& event)
{
	std::string matchId;
	unsigned int teamAScore, teamBScore;
	ExtractMatchInfosFromSelectorValue(event.values[0], matchId, teamAScore, teamBScore);

	std::string userName = event.command.get_issuing_user().global_name;

	const AddBetCommand command{ m_AnswerChannelId, *this, matchId, teamAScore, teamBScore, std::move(userName) };
	event.reply(command.Execute());
	m_Saver.Save(GetDataReader());
}

void BetBot::ExecuteAddResult(const dpp::select_click_t& event)
{
	std::string matchId;
	unsigned int teamAScore, teamBScore;
	ExtractMatchInfosFromSelectorValue(event.values[0], matchId, teamAScore, teamBScore);

	const AddResultCommand command{ m_AnswerChannelId, *this, matchId, teamAScore, teamBScore };
	event.reply(command.Execute());
	m_Saver.Save(GetDataReader());
}

void BetBot::OnNewResult(const std::filesystem::path& file)
{
	if (std::ifstream fileStream{ file.relative_path() };
		fileStream.good()) // does file exists ?
	{
		dpp::json fileContent;
		fileStream >> fileContent;

		if (fileContent.contains("team1score") &&
			fileContent.contains("team2score")
			)
		{
			const std::string teamAScoreStr = fileContent["team1score"];
			const unsigned int teamAScore = std::stol(teamAScoreStr);

			const std::string teamBScoreStr = fileContent["team2score"];
			const unsigned int teamBScore = std::stol(teamBScoreStr);

			const AddResultCommand command{ m_AnswerChannelId, *this, file.stem().string(), teamAScore, teamBScore };
			command.Execute();

			m_Saver.Save(GetDataReader());

			fileStream.close();
		}

		std::filesystem::remove(file);
	}
}

void BetBot::ExecuteShowMatches(const dpp::slashcommand_t& event)
{
	const ShowIncomingMatchesCommand command{ m_AnswerChannelId, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowResults(const dpp::slashcommand_t& event)
{
	const ShowBettorsResultsCommand command{ m_AnswerChannelId, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowBetProposal(const dpp::select_click_t& event)
{
	const ShowBetProposalCommand command{ m_AnswerChannelId, *this, event.values[0] };
	event.reply(command.Execute());
}

void BetBot::ExecuteChooseMatchToBetOn(const dpp::select_click_t& event)
{
	const ChooseMatchToBetOnCommand command{ m_AnswerChannelId, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteChooseMatchToSetResult(const dpp::select_click_t& event)
{
	const ChooseMatchToSetResultCommand command{ m_AnswerChannelId, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowResultProposal(const dpp::select_click_t& event)
{
	const ShowResultProposalCommand command{ m_AnswerChannelId, *this, event.values[0] };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowNextIncomingMatches(const dpp::button_click_t& event)
{
	const std::string lastIndexAsStr = event.custom_id.substr(event.custom_id.find_last_of('-') + 1);
	const size_t lastIndexUsed = std::stol(lastIndexAsStr);
	const ShowIncomingMatchesCommand command{ m_AnswerChannelId, *this, lastIndexUsed };
	event.reply(command.Execute());
}

void BetBot::SetUpCommandCallbacks()
{
	m_Cluster.on_slashcommand(
		[this](const dpp::slashcommand_t& event)
		{
			if (const std::string commandName = event.command.get_command_name();
				commandName == "add_match")
			{
				ExecuteAddMatch(event);
			}
			else if (commandName == "show_matches")
			{
				ExecuteShowMatches(event);
			}
			else if (commandName == "show_results")
			{
				ExecuteShowResults(event);
			}
		}
	);
}

void BetBot::SetUpSelectCallbacks()
{
	m_Cluster.on_select_click(
		[this](const dpp::select_click_t& event)
		{
			if (const std::string selectorId = event.custom_id;
				selectorId == ShowIncomingMatchesCommand::SELECT_MENU_ID)
			{
				const std::string& optionValue = event.values[0];
				if (optionValue == ShowIncomingMatchesCommand::BET_OPTION_VALUE)
				{
					ExecuteChooseMatchToBetOn(event);
				}
				else if (optionValue == ShowIncomingMatchesCommand::RESULT_OPTION_VALUE)
				{
					ExecuteChooseMatchToSetResult(event);
				}
			}
			else if (selectorId == ChooseMatchToBetOnCommand::SELECT_MENU_ID)
			{
				ExecuteShowBetProposal(event);
			}
			else if (selectorId == ShowBetProposalCommand::SELECT_MENU_ID)
			{
				ExecuteAddBet(event);
			}
			else if (selectorId == ChooseMatchToSetResultCommand::SELECT_MENU_ID)
			{
				ExecuteShowResultProposal(event);
			}
			else if (selectorId == ShowResultProposalCommand::SELECT_MENU_ID)
			{
				ExecuteAddResult(event);
			}
		}
	);

	m_Cluster.on_button_click(
		[this](const dpp::button_click_t& event)
		{
			if (const std::string buttonId = event.custom_id;
				buttonId.find(ShowIncomingMatchesCommand::BUTTON_ID) != std::string::npos)
			{
				ExecuteShowNextIncomingMatches(event);
			}
		}
	);
}

void BetBot::SetUpCallbacks()
{
	// Different flows:
	// 1. ExecuteAddMatch
	// 2. ExecuteShowResults
	// 3. ExecuteShowMatches --> ExecuteChooseMatchToBetOn --> ExecuteShowBetProposal --> ExecuteAddBet
	// 4. ExecuteShowMatches --> ExecuteChooseMatchToSetResult --> ExecuteShowResultProposal --> ExecuteAddResult
	SetUpCommandCallbacks();
	SetUpSelectCallbacks();
}

void BetBot::Start()
{
	m_Cluster.start(dpp::st_wait);
}