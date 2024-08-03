#include "BetBot.h"

#include "BotConfigReader.h"


int main()
{
	try
	{
		BetBot bot(BotConfigReader::Read());
		bot.Start();
	}
	catch (const dpp::invalid_token_exception&)
	{
		std::cout << "The bot token given in the config file doesn't work." << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
