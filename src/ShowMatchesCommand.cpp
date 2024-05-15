#include "ShowMatchesCommand.h"

#include <algorithm>

#include "ICommandReceiver.h"
#include "Match.h"

#include <dpp/colors.h>

#include "Bet.h"
#include "DrawUtils.h"

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
}

dpp::message ShowMatchesCommand::ExecuteInternal() const
{
	const std::vector<Match>& matches = m_CommandReceiver.GetIncomingMatches();
	if (matches.empty())
	{
		return { GetAnswerChannelId(), "No match to display yet." };
	}

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

	dpp::message msg{ GetAnswerChannelId(), "Here is the list of incoming matches:" };
	std::ranges::for_each(matches,
		[&msg, &possibleEmbedColors, this](const Match& match)
		{
			msg.add_embed(CreateMatchEmbed(match, possibleEmbedColors));
		}
	);
	msg.add_component(dpp::component().add_component(CreateSelectMenu()));

	return msg;
}

bool ShowMatchesCommand::ValidateCommand(std::string&) const
{
	return true;
}

void ShowMatchesCommand::FillColumnsWithBetsInfos(const Match& match, std::vector<std::vector<std::string>>& outColumnsContent) const
{
	const std::vector<std::reference_wrapper<const Bet>> bets = m_CommandReceiver.GetBetsOnMatch(match.GetId());
	std::ranges::for_each(bets,
		[&outColumnsContent](const Bet& bet)
		{
			outColumnsContent.at(0).emplace_back(bet.GetBettorName());
			outColumnsContent.at(1).emplace_back(bet.GetScore().ToString());
		}
	);
}

dpp::embed ShowMatchesCommand::CreateMatchEmbed(const Match& match, std::vector<uint32_t>& possibleColors) const
{
	dpp::embed result;

	result.set_title(match.GetTeamAName() + " - " + match.GetTeamBName());
	result.set_description("ID: " + std::to_string(match.GetId()) + ", BO" + std::to_string(match.GetBoSize()));
	result.set_color(PopRandomColor(possibleColors));

	std::vector<std::vector<std::string>> columnsContent(TABLE_COLUMN_COUNT);
	FillColumnsWithBetsInfos(match, columnsContent);
	std::string fieldContent;
	DrawUtils::DrawTable(columnsContent, fieldContent);
	result.add_field("Bets:", fieldContent);

	return result;
}