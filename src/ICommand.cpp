#include "ICommand.h"

dpp::message ICommand::Execute(const dpp::slashcommand_t& event) const
{
	if (std::string errorMsg; 
		!ValidateCommand(errorMsg))
	{
		errorMsg = "Error: " + errorMsg;
		return { event.command.channel_id, errorMsg };
	}

	return ExecuteInternal(event);
}
