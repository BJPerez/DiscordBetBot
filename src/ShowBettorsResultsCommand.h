#pragma once

#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class ShowBettorsResultsCommand final : public CommandBase
{
public:
	ShowBettorsResultsCommand(const dpp::snowflake channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] dpp::message Execute() const override;
};

