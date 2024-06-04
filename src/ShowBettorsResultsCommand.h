#pragma once

#include <string>
#include <vector>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class ShowBettorsResultsCommand final : public CommandBase
{
public:
	ShowBettorsResultsCommand(const dpp::snowflake channelId, ICommandReceiver& commandReceiver) : CommandBase(channelId), m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	[[nodiscard]] unsigned int EvaluateMaxBoSize() const noexcept;
	[[nodiscard]] std::vector<std::vector<std::string>> GenerateColumnsWithResultsInfos() const;
};

