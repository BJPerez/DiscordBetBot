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

	//std::istringstream stream("2020-09-16 22:00:00.123456");
	//std::chrono::sys_time<std::chrono::microseconds> tTimePoint;
	//std::chrono::from_stream(stream, "%Y-%m-%d %H:%M:", tTimePoint);

	//std::time_t t_c = std::chrono::system_clock::to_time_t(tTimePoint);
	//std::cout << "Time:  " << std::put_time(std::localtime(&t_c), "%d/%m/%y %H:%M.\n") << std::flush;

	//t_c += 86400;
	//std::cout << "Time:  " << std::put_time(std::localtime(&t_c), "%d/%m/%y %H:%M.\n") << std::flush;

	return EXIT_SUCCESS;
}
