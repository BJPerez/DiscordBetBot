#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class Match;
class ICommandReceiver;

class ShowMatchesCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-What-To-Do";
	static constexpr std::string_view BET_OPTION_VALUE = "Bet";
	static constexpr std::string_view RESULT_OPTION_VALUE = "Result";

	explicit ShowMatchesCommand(const dpp::snowflake channelId, ICommandReceiver& commandReceiver) : CommandBase(channelId), m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	[[nodiscard]] dpp::embed CreateMatchEmbed(const Match& match, std::vector<uint32_t>& possibleColors) const;
};

