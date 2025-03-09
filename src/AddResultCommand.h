#pragma once

#include <string>

#include "CommandBase.h"
#include "MatchScore.h"
#include "MessageBuilder.h"

class ICommandReceiver;

class AddResultCommand final : public CommandBase 
{
public:
	AddResultCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, const ICommandReceiver::AddResultParams& params) noexcept :
			CommandBase(channelId, bot), m_Params(params){}

	MessageBuilder::Message Execute() const override; // NOLINT(modernize-use-nodiscard)

private:
	const ICommandReceiver::AddResultParams& m_Params;
};