#include "ICommand.h"

bool ICommand::Execute(std::string& outAnswerToUser) const
{
	if (std::string errorMsg; 
		!ValidateCommand(errorMsg))
	{
		outAnswerToUser = "Error: " + errorMsg;
		return false;
	}

	ExecuteInternal(outAnswerToUser);
	return true;
}
