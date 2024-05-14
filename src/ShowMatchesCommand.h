#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"

class Match;
class Bet;
class ICommandReceiver;

class ShowMatchesCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-Team-To-Bet";

	explicit ShowMatchesCommand(const dpp::snowflake channelId, ICommandReceiver& commandReceiver) : CommandBase(channelId), m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	[[nodiscard]] dpp::message ExecuteInternal() const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	void FillColumnsWithBetsInfos(const Match& match, std::vector<std::vector<std::string>>& outColumnsContent) const;
	[[nodiscard]] dpp::embed CreateMatchEmbed(const Match& match, std::vector<uint32_t>& possibleColors) const;
};

