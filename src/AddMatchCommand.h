#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class AddMatchCommand final : public CommandBase
{
public:
	AddMatchCommand(const dpp::snowflake channelId, BetBot& bot, std::string teamAName, std::string teamBName, const unsigned int boSize) noexcept :
		CommandBase(channelId, bot), m_TeamAName(std::move(teamAName)), m_TeamBName(std::move(teamBName)), m_BoSize(boSize) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize{ 0 };

	bool ValidateCommand(std::string& outUserErrMsg) const;
};

