#include "ShowMatchesCommand.h"

#include <algorithm>
#include <dpp/colors.h>

#include "Bet.h"
#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

namespace
{
	constexpr unsigned int TABLE_COLUMN_COUNT = 2;
	constexpr uint32_t DEFAULT_COLOR = dpp::colors::black;

	uint32_t PopRandomColor(std::vector<uint32_t>& possibleColors)
	{
		if (possibleColors.empty())
		{
			return DEFAULT_COLOR;
		}

		const int randomIndex = rand() % possibleColors.size();
		const uint32_t result = possibleColors[randomIndex];
		possibleColors.erase(possibleColors.begin() + randomIndex);
		return result;
	}

	dpp::component CreateSelectMenu()
	{
		dpp::component selectMenu;
		selectMenu.set_type(dpp::cot_selectmenu);
		selectMenu.set_placeholder("What do you want to do?");
		selectMenu.add_select_option(dpp::select_option("Place a bet on a match", std::string(ShowMatchesCommand::BET_OPTION_VALUE)));
		selectMenu.add_select_option(dpp::select_option("Add a match result", std::string(ShowMatchesCommand::RESULT_OPTION_VALUE)));
		selectMenu.set_id(std::string(ShowMatchesCommand::SELECT_MENU_ID));

		return selectMenu;
	}

	dpp::embed CreateMatchEmbed(const DataReader<ICommandReceiver>& dataReader, const Match& match, std::vector<uint32_t>& possibleColors)
	{
		dpp::embed result;

		result.set_title(match.GetTeamAName() + " - " + match.GetTeamBName());
		result.set_description("ID: " + std::to_string(match.GetId()) + ", BO" + std::to_string(match.GetBoSize()));
		result.set_color(PopRandomColor(possibleColors));

		std::string fieldContent;
		if (const std::vector<std::reference_wrapper<const Bet>> bets = dataReader->GetBetsOnMatch(match.GetId());
			bets.empty())
		{
			fieldContent = "No bet yet.";
		}
		else
		{
			std::vector<std::vector<std::string>> columnsContent(TABLE_COLUMN_COUNT);
			std::ranges::for_each(bets,
				[&columnsContent](const Bet& bet)
				{
					columnsContent.at(0).emplace_back(bet.GetBettorName());
					columnsContent.at(1).emplace_back(bet.GetScore().ToString());
				}
			);
			DrawUtils::DrawTable(columnsContent, fieldContent);
		}

		result.add_field("Bets:", fieldContent);

		return result;
	}
}

dpp::message ShowMatchesCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataReader dataReader = GetDataReader();
		if (const std::vector<Match>& matches = dataReader->GetIncomingMatches();
			matches.empty())
		{
			msg.set_content("No match to display yet.");
		}
		else
		{
			msg.set_content("Here is the list of incoming matches:");

			std::vector<uint32_t> possibleEmbedColors =
			{
				dpp::colors::red,
				dpp::colors::dark_orange,
				dpp::colors::yellow,
				dpp::colors::green,
				dpp::colors::cyan,
				dpp::colors::blue,
				dpp::colors::brown,
				dpp::colors::silver,
				dpp::colors::sangria,
				dpp::colors::jade
			};

			std::ranges::for_each(matches,
				[&dataReader, &msg, &possibleEmbedColors, this](const Match& match)
				{
					msg.add_embed(CreateMatchEmbed(dataReader, match, possibleEmbedColors));
				}
			);
			msg.add_component(dpp::component().add_component(CreateSelectMenu()));
		}
	}

	return msg;
}