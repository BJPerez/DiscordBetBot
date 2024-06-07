#include "CommandBase.h"

 dpp::message CommandBase::Execute() const
 {
	 if (std::string errorMsg;
		 !ValidateCommand(errorMsg))
	 {
		dpp::message msg{ m_AnswerChannelId, "Error: " + errorMsg };
		msg.set_flags(dpp::m_ephemeral);
		return msg;
	 }

	 return ExecuteInternal();
 }