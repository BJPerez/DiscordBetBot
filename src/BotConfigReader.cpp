#include "BotConfigReader.h"

#include <dpp/json.h>
#include <fstream>

namespace
{
	std::ifstream OpenConfigFile()
	{
		std::ifstream configFile{ std::string(BotConfigReader::CONFIG_PATH) };
		if (!configFile.good())
		{
			throw BotConfigReader::ConfigErrorException("Coudn't find the config file. It has to be named \"config.json\" and placed next to the executable.");
		}

		return configFile;
	}

	dpp::json GetAsJsonAndValidateKeys(std::ifstream& fileStream)
	{
		dpp::json config;
		fileStream >> config;

		if (!config.contains(BotConfigReader::BOT_TOKEN_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::BOT_TOKEN_KEY));
		}

		if (!config.contains(BotConfigReader::CHANNEL_ID_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::CHANNEL_ID_KEY));
		}
			
		if (!config.contains(BotConfigReader::SAVE_FILE_PATH_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::SAVE_FILE_PATH_KEY));
		}

		if (!config.contains(BotConfigReader::NEW_MATCHES_FOLDER_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::NEW_MATCHES_FOLDER_KEY));
		}

		if (!config.contains(BotConfigReader::NEW_RESULTS_FOLDER_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::NEW_RESULTS_FOLDER_KEY));
		}

		if (!config.contains(BotConfigReader::DELAY_BETWEEN_CHECKS_KEY))
		{
			throw BotConfigReader::MissingConfigKeyException(std::string(BotConfigReader::DELAY_BETWEEN_CHECKS_KEY));
		}

		return config;
	}

	void ReadBotToken(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		std::string botToken = config[BotConfigReader::BOT_TOKEN_KEY];
		if (botToken.empty())
		{
			throw BotConfigReader::EmptyConfigValueException(std::string(BotConfigReader::BOT_TOKEN_KEY));
		}

		outResults.m_BotToken = std::move(botToken);
	}

	void ReadAnswerChannelId(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		std::string chanelId = config[BotConfigReader::CHANNEL_ID_KEY];
		if (chanelId.empty())
		{
			throw BotConfigReader::EmptyConfigValueException(std::string(BotConfigReader::CHANNEL_ID_KEY));
		}

		outResults.m_AnswerChannelId = std::move(chanelId);
	}

	void ReadSaveFilePath(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		std::string saveFile = config[BotConfigReader::SAVE_FILE_PATH_KEY];
		if (saveFile.empty())
		{
			throw BotConfigReader::EmptyConfigValueException(std::string(BotConfigReader::SAVE_FILE_PATH_KEY));
		}

		if (const std::string saveExtension = saveFile.substr(saveFile.find_last_of('.'));
			saveExtension != BotConfigReader::SAVE_FILE_EXTENSION)
		{
			throw BotConfigReader::ConfigErrorException("Given save file path has the wrong extension. It should be .json");
		}

		outResults.m_SaveFilePath = std::move(saveFile);
	}

	void ReadNewMatchesFolder(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		if (std::string newMatchesFolder = config[BotConfigReader::NEW_MATCHES_FOLDER_KEY]; 
			!newMatchesFolder.empty())
		{
			outResults.m_NewMatchesFolder.emplace(std::move(newMatchesFolder));
		}
	}

	void ReadNewResultsFolder(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		if (std::string newResultsFolder = config[BotConfigReader::NEW_RESULTS_FOLDER_KEY];
			!newResultsFolder.empty())
		{
			outResults.m_NewResultsFolder.emplace(std::move(newResultsFolder));
		}
	}

	void ReadDelayBetweenChecks(const dpp::json& config, BotConfigReader::Results& outResults)
	{
		if (std::string delayBetweenChecks = config[BotConfigReader::DELAY_BETWEEN_CHECKS_KEY];
			!delayBetweenChecks.empty())
		{
			outResults.m_DelayBetweenChecks.emplace(std::stoi(std::move(delayBetweenChecks)));
		}
	}
}

BotConfigReader::Results BotConfigReader::Read()
{
	std::ifstream configFile = OpenConfigFile();
	const dpp::json config = GetAsJsonAndValidateKeys(configFile);

	Results results;
	ReadBotToken(config, results);
	ReadAnswerChannelId(config, results);
	ReadSaveFilePath(config, results);
	ReadNewMatchesFolder(config, results);
	ReadNewResultsFolder(config, results);
	ReadDelayBetweenChecks(config, results);

	return results;
}