#pragma once

#include <string_view>

#include "CommandBase.h"
#include "MessageBuilder.h"

class ShowIncomingMatchesCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-What-To-Do";
	static constexpr std::string_view BET_OPTION_VALUE = "Bet";
	static constexpr std::string_view RESULT_OPTION_VALUE = "Result";

	ShowIncomingMatchesCommand(const MessageBuilder::ChannelId channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;
};

