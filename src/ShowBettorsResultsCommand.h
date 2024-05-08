#pragma once

#include <string>
#include <vector>

#include "ICommand.h"

class ICommandReceiver;

class ShowBettorsResultsCommand final : public ICommand
{
public:
	explicit ShowBettorsResultsCommand(ICommandReceiver& commandReceiver) : m_CommandReceiver(commandReceiver) {}

private:
	ICommandReceiver& m_CommandReceiver;

	dpp::message ExecuteInternal(const dpp::slashcommand_t& event) const final;
	[[nodiscard]] bool ValidateCommand(std::string& outUserErrMsg) const final;

	void FillColumnsWithResultsInfos(std::vector<std::vector<std::string>>& outColumnsContent) const;
};

