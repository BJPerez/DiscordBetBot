#pragma once

#include <optional>
#include <string>

#include "CommandBase.h"
#include "MessageBuilder.h"

class AddMatchCommand final : public CommandBase
{
public:
	AddMatchCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, std::string teamAName, std::string teamBName, const unsigned int boSize, 
		std::string timeAsString, std::optional<std::string> matchId = std::optional<std::string>()) noexcept :
			CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_TeamAName(std::move(teamAName)),
			m_TeamBName(std::move(teamBName)), m_BoSize(boSize), m_DateTimeAsString(std::move(timeAsString))
	{}

	MessageBuilder::Message Execute() const override;  // NOLINT(modernize-use-nodiscard)

private:
	std::optional<std::string> m_MatchId;
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize{ 0 };
	std::string m_DateTimeAsString;
};

