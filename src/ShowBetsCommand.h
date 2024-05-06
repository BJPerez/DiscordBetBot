#pragma once

#include <vector>

#include "ICommand.h"

class Bet;
class ICommandReceiver;

class ShowBetsCommand final : public ICommand
{
public:
	explicit ShowBetsCommand(ICommandReceiver& commandReceiver): m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	void ExecuteInternal(std::string& outAnswerToUser) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	void FillColumnsWithBetInfos(std::vector<std::vector<std::string>>& outColumnsContent) const;
};

