#pragma once

#include <optional>
#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class AddMatchCommand final : public CommandBase
{
public:
	AddMatchCommand(const dpp::snowflake channelId, BetBot& bot, std::string teamAName, std::string teamBName, const unsigned int boSize, 
		std::optional<std::string> matchId = std::optional<std::string>()) noexcept : CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_TeamAName(std::move(teamAName)),
		m_TeamBName(std::move(teamBName)), m_BoSize(boSize) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	std::optional<std::string> m_MatchId;
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize{ 0 };
};

