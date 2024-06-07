#include "ShowBetProposalCommand.h"

#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "Match.h"

dpp::message ShowBetProposalCommand::ExecuteInternal() const
{
	const Match& match = m_CommandReceiver.GetMatch(m_MatchId).value(); // ValidateCommand already checks that the match exists

	dpp::message msg{ GetAnswerChannelId(), "Choose your bet for " + match.GetTeamAName() + " - " + match.GetTeamBName() + ":" };
	msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchResultSelector(match.GetTeamAName() + " - " + match.GetTeamBName(), std::string(SELECT_MENU_ID), match)));
	msg.set_flags(dpp::m_ephemeral);

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