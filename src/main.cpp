#include "BetBot.h"

#include <fstream>

constexpr std::string_view CONFIG_PATH = "config.json";
constexpr std::string_view BOT_TOKEN_KEY = "BotToken";
constexpr std::string_view SAVE_FILE_KEY = "SaveFile";
constexpr std::string_view SAVE_FILE_EXTENSION = ".json";

int main()
{
	std::ifstream configFile{ std::string(CONFIG_PATH) };
	if (!configFile.good()) // does file exists ?
	{
		std::cout << "No config file." << std::endl;
		return EXIT_FAILURE;
	} 

	dpp::json config;
	configFile >> config;
	if (!config.contains(BOT_TOKEN_KEY) || !config.contains(SAVE_FILE_KEY))
	{
		std::cout << "Invalid config file" << std::endl;
		return EXIT_FAILURE;
	}

	std::string saveFile = config[SAVE_FILE_KEY];
	if (saveFile.empty())
	{
		std::cout << "No save file given in config file." << std::endl;
		return EXIT_FAILURE;
	}

	if (const std::string saveExtension = saveFile.substr(saveFile.find_last_of('.')); 
		saveExtension != SAVE_FILE_EXTENSION)
	{
		std::cout << "Given save file has extension " << saveExtension << " but it must be a .json file." << std::endl;
		return EXIT_FAILURE;
	}

	BetBot bot(config[BOT_TOKEN_KEY], std::move(saveFile));
	bot.Start();

	return EXIT_SUCCESS;
}
