#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class AddMatchCommand final : public CommandBase
{
public:
	explicit AddMatchCommand(const dpp::snowflake channelId, std::string teamAName, std::string teamBName, const unsigned int boSize, ICommandReceiver& commandReceiver):
		CommandBase(channelId), m_TeamAName(std::move(teamAName)), m_TeamBName(std::move(teamBName)), m_BoSize(boSize), m_CommandReceiver(commandReceiver) {}

private:
	std::string m_TeamAName;
	std::string m_TeamBName;
	unsigned int m_BoSize{ 0 };
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};

