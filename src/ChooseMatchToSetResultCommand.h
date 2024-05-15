#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class ICommandReceiver;

class ChooseMatchToSetResultCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Match-To-Set-Result";

	explicit ChooseMatchToSetResultCommand(const dpp::snowflake channelId, ICommandReceiver& commandReceiver) : CommandBase(channelId), m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;
};
