#pragma once

#include <string>
#include <dpp/message.h>
#include <dpp/snowflake.h>

#include "CommandBase.h"
#include "Match.h"
#include "MatchScore.h"

class ICommandReceiver;

class AddBetCommand final : public CommandBase
{
public:
	AddBetCommand(const dpp::snowflake channelId, BetBot& bot, std::string matchId, const unsigned int teamAScore, const unsigned int teamBScore, std::string bettorName) noexcept:
		CommandBase(channelId, bot), m_MatchId(std::move(matchId)), m_Score{ teamAScore, teamBScore }, m_BettorName(std::move(bettorName)) {}

	[[nodiscard]] dpp::message Execute() const override;

private:
	std::string m_MatchId{ Match::INVALID_ID };
	MatchScore m_Score;
	std::string m_BettorName;

	bool ValidateCommand(const DataWriter<ICommandReceiver>& dataWriter, std::string& outUserErrMsg) const;
};

