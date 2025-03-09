#pragma once

#include <optional>
#include <string>

#include "CommandBase.h"
#include "MessageBuilder.h"

class AddMatchCommand final : public CommandBase
{
public:
	AddMatchCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, const ICommandReceiver::AddMatchParams& params) noexcept :
			CommandBase(channelId, bot), m_Params(params) {}

	MessageBuilder::Message Execute() const override;  // NOLINT(modernize-use-nodiscard)

private:
	const ICommandReceiver::AddMatchParams& m_Params;
};

