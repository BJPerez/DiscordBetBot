#include "BetBot.h"

#include "AddBetCommand.h"
#include "AddMatchCommand.h"
#include "AddResultCommand.h"
#include "ShowBetsCommand.h"
#include "ShowBettorsResultsCommand.h"
#include "ShowMatchesCommand.h"

BetBot::BetBot(const std::string& betToken, std::string saveFilePath): m_Cluster(betToken), m_Data(), m_Saver(std::move(saveFilePath), m_Data)
{
	m_Saver.Load();
	m_Cluster.on_log(dpp::utility::cout_logger());

	CreateCommands();
	SetUpCallbacks();
}

dpp::slashcommand BetBot::CreateAddMatchCommand() const
{
	dpp::slashcommand command("add_match", "Add an incoming match.", m_Cluster.me.id);
	command.add_option(dpp::command_option(dpp::co_string, "team_a", "The name of the first team.", true));
	command.add_option(dpp::command_option(dpp::co_string, "team_b", "The name of the second team.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "bo_size", "The maximal number of game of this match.", true));

	return command;
}

dpp::slashcommand BetBot::CreateAddBetCommand() const
{
	dpp::slashcommand command("add_bet", "Add a bet for a match.", m_Cluster.me.id);
	command.add_option(dpp::command_option(dpp::co_integer, "match_id", "The ID of the match on which you want to bet. You can use /ShowMatch to see it.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "score_a", "The first team's score.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "score_b", "The second team's score.", true));

	return command;
}

dpp::slashcommand BetBot::CreateAddResultCommand() const
{
	dpp::slashcommand command("add_result", "Add a result for a match and evaluate the scores of the bettors.", m_Cluster.me.id);
	command.add_option(dpp::command_option(dpp::co_integer, "match_id", "The ID of the match on which you want to set a result. You can use /show_matches to see it.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "score_a", "The first team's score.", true));
	command.add_option(dpp::command_option(dpp::co_integer, "score_b", "The second team's score.", true));

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
				commands.emplace_back(CreateAddBetCommand());
				commands.emplace_back(CreateAddResultCommand());
				commands.emplace_back(CreateShowMatchesCommand());
				commands.emplace_back(CreateShowBetsCommand());
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

	const AddMatchCommand command{ std::move(teamAName), std::move(teamBName), boSize, m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
	m_Saver.Save();
}

void BetBot::ExecuteAddBet(const dpp::slashcommand_t& event)
{
	const unsigned int matchId = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("match_id")));
	const unsigned int teamAScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_a")));
	const unsigned int teamBScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_b")));
	std::string userName = event.command.get_issuing_user().global_name;

	const AddBetCommand command{ matchId, teamAScore, teamBScore, std::move(userName), m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
	m_Saver.Save();
}

void BetBot::ExecuteAddResult(const dpp::slashcommand_t& event)
{
	const unsigned int matchId = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("match_id")));
	const unsigned int teamAScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_a")));
	const unsigned int teamBScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_b")));

	const AddResultCommand command{ matchId, teamAScore, teamBScore, m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
	m_Saver.Save();
}

void BetBot::ExecuteShowMatches(const dpp::slashcommand_t& event)
{
	const ShowMatchesCommand command{ m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
}

void BetBot::ExecuteShowBets(const dpp::slashcommand_t& event)
{
	const ShowBetsCommand command{ m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
}

void BetBot::ExecuteShowResults(const dpp::slashcommand_t& event)
{
	const ShowBettorsResultsCommand command{ m_Data };
	std::string answer;
	command.Execute(answer);
	event.reply(answer);
}

void BetBot::SetUpCallbacks()
{
	/* Handle slash command with the most recent addition to D++ features, coroutines! */
	m_Cluster.on_slashcommand(
		[this](const dpp::slashcommand_t& event)
		{
			if (const std::string commandName = event.command.get_command_name(); 
				commandName == "add_match")
			{
				ExecuteAddMatch(event);
			}
			else if (commandName == "add_bet")
			{
				ExecuteAddBet(event);
			}
			else if (commandName == "add_result")
			{
				ExecuteAddResult(event);
			}
			else if (commandName == "show_matches")
			{
				ExecuteShowMatches(event);
			}
			else if (commandName == "show_bets")
			{
				ExecuteShowBets(event);
			}
			else if (commandName == "show_results")
			{
				ExecuteShowResults(event);
			}
		}
	);
}

void BetBot::Start()
{
	m_Cluster.start(dpp::st_wait);
}