#include "CommandBase.h"

 dpp::message CommandBase::Execute() const
 {
	 if (std::string errorMsg;
		 !ValidateCommand(errorMsg))
	 {
		 return { m_AnswerChannelId, "Error: " + errorMsg };
	 }

	 return ExecuteInternal();
 }