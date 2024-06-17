#pragma once

#include <string_view>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class ChooseMatchToSetResultCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Match-To-Set-Result";

	ChooseMatchToSetResultCommand(const dpp::snowflake channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] dpp::message Execute() const override;
};
