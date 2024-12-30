#pragma once

#include <string_view>

#include "CommandBase.h"
#include "MessageBuilder.h"

class BetBot;

class ChooseMatchToSetResultCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Match-To-Set-Result";

	ChooseMatchToSetResultCommand(const MessageBuilder::ChannelId channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;
};
