#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"
#include "Match.h"

class ICommandReceiver;

class ShowBetProposalCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Bet";

	explicit ShowBetProposalCommand(const dpp::snowflake channelId, const unsigned int matchId, ICommandReceiver& commandReceiver) : CommandBase(channelId), m_MatchId(matchId),
		m_CommandReceiver(commandReceiver) {}

private:
	unsigned int m_MatchId{ Match::INVALID_ID };
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};
