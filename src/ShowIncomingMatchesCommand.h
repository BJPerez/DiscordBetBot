#pragma once

#include <string_view>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class Match;
class ICommandReceiver;

class ShowIncomingMatchesCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-What-To-Do";
	static constexpr std::string_view BET_OPTION_VALUE = "Bet";
	static constexpr std::string_view RESULT_OPTION_VALUE = "Result";

	ShowIncomingMatchesCommand(const dpp::snowflake channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] dpp::message Execute() const override;
};

