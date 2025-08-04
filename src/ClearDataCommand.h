#pragma once

#include "CommandBase.h"
#include "MessageBuilder.h"

class ClearDataCommand final : public CommandBase
{
public:
	ClearDataCommand(const MessageBuilder::ChannelId channelId, BetBot& bot) noexcept :
		CommandBase(channelId, bot)
	{
	}

	[[nodiscard]] MessageBuilder::Message Execute() const override;
};