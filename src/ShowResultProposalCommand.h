#pragma once

#include <string>
#include <string_view>

#include "CommandBase.h"
#include "MessageBuilder.h"

class ShowResultProposalCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Result";

	ShowResultProposalCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, std::string matchId) noexcept : CommandBase(channelId, bot), m_MatchId(std::move(matchId)) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;

private:
	std::string m_MatchId;
};
