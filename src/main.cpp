#include "BetBot.h"

int main(const int argc, char* argv[])
{
	// Index 0 --> exe
	// Index 1 --> bot token
	// Index 2 --> path to save file
	if (argc != 3)
	{
		return EXIT_FAILURE;
	}

	BetBot bot(argv[1], argv[2]);
	bot.Start();

	return EXIT_SUCCESS;
}
