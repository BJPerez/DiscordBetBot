#pragma once

#include "CommandBase.h"
#include "MessageBuilder.h"

class ICommandReceiver;

class ShowBettorsResultsCommand final : public CommandBase
{
public:
	ShowBettorsResultsCommand(const MessageBuilder::ChannelId channelId, BetBot& bot) noexcept : CommandBase(channelId, bot) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;
};

