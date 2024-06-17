#include "ShowBetProposalCommand.h"

#include "DrawUtils.h"
#include "ICommandReceiver.h"
#include "LockableDataAccessors.h"
#include "Match.h"

dpp::message ShowBetProposalCommand::Execute() const
{
	dpp::message msg{ GetAnswerChannelId(), "" };
	msg.set_flags(dpp::m_ephemeral);

	{
		const DataReader dataReader = GetDataReader();
		if (std::string errorMsg;
			!ValidateCommand(dataReader, errorMsg))
		{
			msg.set_content("Error: " + errorMsg);
		}
		else
		{
			const Match& match = dataReader->GetMatch(m_MatchId).value(); // ValidateCommand already checks that the match exists
			msg.set_content("Choose your bet for " + match.GetTeamAName() + " - " + match.GetTeamBName() + ":");
			msg.add_component(dpp::component().add_component(DrawUtils::CreateMatchResultSelector(match.GetTeamAName() + " - " 
				+ match.GetTeamBName(), std::string(SELECT_MENU_ID), match)));
		}
	}

	return msg;
}

bool ShowBetProposalCommand::ValidateCommand(const DataReader<ICommandReceiver>& dataReader, std::string& outUserErrMsg) const
{
	if (const std::optional<std::reference_wrapper<const Match>> matchOpt = dataReader->GetMatch(m_MatchId);
		!matchOpt.has_value())
	{
		return false;
	}

	return true;
}