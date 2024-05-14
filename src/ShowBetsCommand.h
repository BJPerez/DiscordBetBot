#pragma once

#include <string>
#include <vector>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class Bet;
class ICommandReceiver;

class ShowBetsCommand final : public CommandBase
{
public:
	explicit ShowBetsCommand(const dpp::snowflake channelId, ICommandReceiver& commandReceiver): CommandBase(channelId), m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	void FillColumnsWithBetInfos(std::vector<std::vector<std::string>>& outColumnsContent) const;
};

