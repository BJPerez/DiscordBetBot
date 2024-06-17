#include "BetBot.h"

#include "AddBetCommand.h"
#include "AddMatchCommand.h"
#include "AddResultCommand.h"
#include "ChooseMatchToBetOnCommand.h"
#include "ChooseMatchToSetResultCommand.h"
#include "ShowBetProposalCommand.h"
#include "ShowBettorsResultsCommand.h"
#include "ShowMatchesCommand.h"
#include "ShowResultProposalCommand.h"

namespace
{
	void ExtractMatchInfosFromSelectorValue(std::string selectorValue, unsigned int& outMatchId, unsigned int& outTeamAScore, unsigned int& outTeamBScore)
	{
		// format of the value is "MatchId-AScore-BScore"

		std::size_t delimiter = selectorValue.find('-');
		outMatchId = std::stoul(selectorValue.substr(0, delimiter));
		selectorValue = selectorValue.substr(delimiter + 1);

		delimiter = selectorValue.find("-");
		outTeamAScore = std::stoul(selectorValue.substr(0, delimiter));
		selectorValue = selectorValue.substr(delimiter + 1);

		outTeamBScore = std::stoul(selectorValue);
	}
}

BetBot::BetBot(const std::string& betToken, std::string saveFilePath): m_Cluster(betToken), m_Data(), m_Saver(std::move(saveFilePath), m_Data)
{
	m_Saver.Load();
	m_Cluster.on_log(dpp::utility::cout_logger());

	CreateCommands();
	SetUpCallbacks();

	srand(time(nullptr)); // Init randomness
}

dpp::slashcommand BetBot::CreateAddMatchCommand() const
{
	dpp::slashcommand command("add_match", "Add an incoming match.", m_Cluster.me.id);
	command.add_option(dpp::command_option(dpp::co_string, "team_a", "The name of the first team.", true));
	command.add_option(dpp::command_option(dpp::co_string, "team_b", "The name of the second team.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "bo_size", "The maximal number of game of this match.", true));

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

	const AddMatchCommand command{ event.command.channel_id, *this, std::move(teamAName), std::move(teamBName), boSize };
	event.reply(command.Execute());
	m_Saver.Save();
}

void BetBot::ExecuteAddBet(const dpp::select_click_t& event)
{
	unsigned int matchId, teamAScore, teamBScore;
	ExtractMatchInfosFromSelectorValue(event.values[0], matchId, teamAScore, teamBScore);

	std::string userName = event.command.get_issuing_user().global_name;

	const AddBetCommand command{ event.command.channel_id, *this, matchId, teamAScore, teamBScore, std::move(userName) };
	event.reply(command.Execute());
	m_Saver.Save();
}

void BetBot::ExecuteAddResult(const dpp::select_click_t& event)
{
	unsigned int matchId, teamAScore, teamBScore;
	ExtractMatchInfosFromSelectorValue(event.values[0], matchId, teamAScore, teamBScore);

	const AddResultCommand command{ event.command.channel_id, *this, matchId, teamAScore, teamBScore };
	event.reply(command.Execute());
	m_Saver.Save();
}

void BetBot::ExecuteShowMatches(const dpp::slashcommand_t& event)
{
	const ShowMatchesCommand command{ event.command.channel_id, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowResults(const dpp::slashcommand_t& event)
{
	const ShowBettorsResultsCommand command{ event.command.channel_id, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowBetProposal(const dpp::select_click_t& event)
{
	const ShowBetProposalCommand command{ event.command.channel_id, *this, static_cast<unsigned int>(std::stoul(event.values[0])) };
	event.reply(command.Execute());
}

void BetBot::ExecuteChooseMatchToBetOn(const dpp::select_click_t& event)
{
	const ChooseMatchToBetOnCommand command{ event.command.channel_id, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteChooseMatchToSetResult(const dpp::select_click_t& event)
{
	const ChooseMatchToSetResultCommand command{ event.command.channel_id, *this };
	event.reply(command.Execute());
}

void BetBot::ExecuteShowResultProposal(const dpp::select_click_t& event)
{
	const ShowResultProposalCommand command{ event.command.channel_id, *this, static_cast<unsigned int>(std::stoul(event.values[0])) };
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
				selectorId == ShowMatchesCommand::SELECT_MENU_ID)
			{
				const std::string& optionValue = event.values[0];
				if (optionValue == ShowMatchesCommand::BET_OPTION_VALUE)
				{
					ExecuteChooseMatchToBetOn(event);
				}
				else if (optionValue == ShowMatchesCommand::RESULT_OPTION_VALUE)
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