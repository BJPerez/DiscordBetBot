#include "BetBot.h"

#include "BotConfigReader.h"

int main()
{
	try
	{
		BetBot bot(BotConfigReader::Read());
		bot.Start();
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
