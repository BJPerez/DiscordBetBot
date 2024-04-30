#include <dpp/dpp.h>

#include "AddBetCommand.h"
#include "AddMatchCommand.h"
#include "AddResultCommand.h"
#include "BotData.h"
#include "JsonSerializer.h"
#include "SaveManager.h"
#include "ShowBetsCommand.h"
#include "ShowBettorsResultsCommand.h"
#include "ShowMatchesCommand.h"

int main(const int argc, char* argv[])
{
	// Index 0 --> exe
	// Index 1 --> bot token
	// Index 2 --> path to save file
	if (argc != 3)
	{
		return EXIT_FAILURE;
	}

	/* Create bot cluster */
	dpp::cluster bot(argv[1]);
	BotData botData;
	SaveManager saveManager(argv[2], botData);
	saveManager.Load();

	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());

	/* Register slash command here in on_ready */
	bot.on_ready(
		[&bot](const dpp::ready_t&)
		{
			/* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
			if (dpp::run_once<struct register_bot_commands>())
			{
				dpp::slashcommand addMatchCommand("add_match", "Add an incoming match.", bot.me.id);
				addMatchCommand.add_option(dpp::command_option(dpp::co_string, "team_a", "The name of the first team.", true));
				addMatchCommand.add_option(dpp::command_option(dpp::co_string, "team_b", "The name of the second team.", true));
				addMatchCommand.add_option(dpp::command_option(dpp::co_integer, "bo_size", "The maximal number of game of this match.", true));

				dpp::slashcommand addBetCommand("add_bet", "Add a bet for a match.", bot.me.id);
				addBetCommand.add_option(dpp::command_option(dpp::co_integer, "match_id", "The ID of the match on which you want to bet. You can use /ShowMatch to see it.", true));
				addBetCommand.add_option(dpp::command_option(dpp::co_integer, "score_a", "The first team's score.", true));
				addBetCommand.add_option(dpp::command_option(dpp::co_integer, "score_b", "The second team's score.", true));

				dpp::slashcommand addResultCommand("add_result", "Add a result for a match and evaluate the scores of the bettors.", bot.me.id);
				addResultCommand.add_option(dpp::command_option(dpp::co_integer, "match_id", "The ID of the match on which you want to set a result. You can use /show_matches to see it.", true));
				addResultCommand.add_option(dpp::command_option(dpp::co_integer, "score_a", "The first team's score.", true));
				addResultCommand.add_option(dpp::command_option(dpp::co_integer, "score_b", "The second team's score.", true));

				dpp::slashcommand showMatchesCommand("show_matches", "Show the incoming matches on which you can bet.", bot.me.id);
				dpp::slashcommand showBetsCommand("show_bets", "Show the different ongoing bets.", bot.me.id);
				dpp::slashcommand showResultsCommand("show_results", "Show the different bettors and their results.", bot.me.id);

				bot.global_bulk_command_create({ addMatchCommand, addBetCommand, addResultCommand, showMatchesCommand, showBetsCommand, showResultsCommand });
			}
		}
	);

	/* Handle slash command with the most recent addition to D++ features, coroutines! */
	bot.on_slashcommand(
		[&botData, &saveManager](const dpp::slashcommand_t& event)
		{
			if (event.command.get_command_name() == "add_match")
			{
				auto teamAName = std::get<std::string>(event.get_parameter("team_a"));
				auto teamBName = std::get<std::string>(event.get_parameter("team_b"));
				const unsigned int boSize = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("bo_size")));

				const AddMatchCommand command{ std::move(teamAName), std::move(teamBName), boSize, botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
				saveManager.Save();
			}
			else if (event.command.get_command_name() == "add_bet")
			{
				const unsigned int matchId = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("match_id")));
				const unsigned int teamAScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_a")));
				const unsigned int teamBScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_b")));
				std::string userName = event.command.get_issuing_user().global_name;

				const AddBetCommand command{ matchId, teamAScore, teamBScore, std::move(userName), botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
				saveManager.Save();
			}
			else if (event.command.get_command_name() == "add_result")
			{
				const unsigned int matchId = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("match_id")));
				const unsigned int teamAScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_a")));
				const unsigned int teamBScore = static_cast<unsigned int>(std::get<int64_t>(event.get_parameter("score_b")));

				const AddResultCommand command{ matchId, teamAScore, teamBScore, botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
				saveManager.Save();
			}
			else if (event.command.get_command_name() == "show_matches")
			{
				const ShowMatchesCommand command{ botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
			}
			else if (event.command.get_command_name() == "show_bets")
			{
				const ShowBetsCommand command{ botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
			}
			else if (event.command.get_command_name() == "show_results")
			{
				const ShowBettorsResultsCommand command{ botData };
				std::string answer;
				command.Execute(answer);
				event.reply(answer);
			}
		}
	);

	/* Start the bot */
	bot.start(dpp::st_wait);

	return EXIT_SUCCESS;
}
