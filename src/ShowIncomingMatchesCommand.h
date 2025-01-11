#pragma once

#include <string_view>

#include "CommandBase.h"
#include "MessageBuilder.h"

class ShowIncomingMatchesCommand final : public CommandBase
{
public:
	static constexpr std::string_view SELECT_MENU_ID = "Choose-What-To-Do";
	static constexpr std::string_view BET_OPTION_VALUE = "Bet";
	static constexpr std::string_view RESULT_OPTION_VALUE = "Result";
	static constexpr std::string_view BUTTON_ID = "Show-Next-Matches-";

	ShowIncomingMatchesCommand(const MessageBuilder::ChannelId channelId, BetBot& bot, const size_t lastIndex = 0) noexcept :
		CommandBase(channelId, bot), m_LastIndexUsed(lastIndex) {}

	[[nodiscard]] MessageBuilder::Message Execute() const override;

private:
	size_t m_LastIndexUsed;

	size_t AddMatchesToMsg(const DataReader<ICommandReceiver>& dataReader, const std::vector<std::reference_wrapper<const Match>>& matches, 
		MessageBuilder::Message& outMsg) const;
};

