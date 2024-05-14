#include "ShowBetProposalCommand.h"

#include "ICommandReceiver.h"
#include "Match.h"
#include "MatchScore.h"

namespace
{
	dpp::component CreateSelectMenu(const Match& match, const std::vector<std::pair<std::string, std::string>>& options)
	{
		dpp::component selectMenu;
		selectMenu.set_type(dpp::cot_selectmenu);
		selectMenu.set_placeholder(match.GetTeamAName() + " - " + match.GetTeamBName());
		std::ranges::for_each(options,
			[&selectMenu](const std::pair<std::string, std::string>& option)
			{
				selectMenu.add_select_option(dpp::select_option(option.first, option.second));
			}
		);
		selectMenu.set_id(std::string(ShowBetProposalCommand::SELECT_MENU_ID));

		return selectMenu;
	}

	std::pair<std::string, std::string> CreateSelectMenuOption(const unsigned int matchId, const MatchScore& score)
	{
		std::string displayedOption = score.ToString();
		std::string optionValue = std::to_string(matchId) + "-" + std::to_string(score.m_TeamAScore) + "-" + std::to_string(score.m_TeamBScore); // score.ToString has space around the '-' that we don't want in the value
		return { std::move(displayedOption), std::move(optionValue) };
	}
}

dpp::message ShowBetProposalCommand::ExecuteInternal() const
{
	const Match& match = m_CommandReceiver.GetMatch(m_MatchId).value(); // ValidateCommand already checks that the bet exists

	std::vector<std::pair<std::string, std::string>> selectMenuOptions;
	const unsigned int winningScore = match.GetNumberOfGamesToWin();
	for (unsigned int looserScore = 0; looserScore < winningScore; ++looserScore )
	{
		selectMenuOptions.emplace_back(CreateSelectMenuOption(m_MatchId, { winningScore, looserScore }));
		selectMenuOptions.emplace_back(CreateSelectMenuOption(m_MatchId, { looserScore, winningScore }));
	}

	dpp::message msg{ GetAnswerChannelId(), "You chose to bet on " + match.GetTeamAName() + " - " + match.GetTeamBName() + "\nNow, choose the result:" };
	msg.add_component(dpp::component().add_component(CreateSelectMenu(match, selectMenuOptions)));

	return msg;
}

bool ShowBetProposalCommand::ValidateCommand(std::string& outUserErrMsg) const
{
	if (const std::optional<std::reference_wrapper<const Match>> matchOpt = m_CommandReceiver.GetMatch(m_MatchId); 
		!matchOpt.has_value())
	{
		return false;
	}

	return true;
}