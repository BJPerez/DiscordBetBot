#include "ClearDataCommand.h"

MessageBuilder::Message ClearDataCommand::Execute() const
{
	const DataWriter dataWriter = GetDataWriter();
	dataWriter->ClearData();

	return MessageBuilder::BuildAnswer(GetAnswerChannelId(), "Data cleared.");
}