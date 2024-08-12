#pragma once

#include <chrono>
#include <exception>
#include <optional>
#include <string>
#include <string_view>
#include <dpp/snowflake.h>

namespace BotConfigReader
{
	constexpr std::string_view CONFIG_PATH = "config.json";
	constexpr std::string_view BOT_TOKEN_KEY = "BotToken";
	constexpr std::string_view CHANNEL_ID_KEY = "ChannelId";
	constexpr std::string_view SAVE_FILE_PATH_KEY = "SaveFile";
	constexpr std::string_view NEW_MATCHES_FOLDER_KEY = "NewMatchesFolder";
	constexpr std::string_view NEW_RESULTS_FOLDER_KEY = "NewResultsFolder";
	constexpr std::string_view DELAY_BETWEEN_CHECKS_KEY = "DelayBetweenChecks";
	constexpr std::string_view SAVE_FILE_EXTENSION = ".json";

	struct Results
	{
		std::string m_BotToken;
		dpp::snowflake m_AnswerChannelId; 
		std::string m_SaveFilePath;
		std::optional<std::string> m_NewMatchesFolder;
		std::optional<std::string> m_NewResultsFolder;
		std::optional<std::chrono::minutes> m_DelayBetweenChecks; // For automatic detection of new results and new matches
	};

	class ConfigErrorException final : public std::exception 
	{
	public:
		explicit ConfigErrorException(std::string what) noexcept : m_What(std::move(what)) {};

		[[nodiscard]] const char* what() const noexcept override
		{
			return m_What.data();
		}

	private:
		std::string m_What;
	};

	class MissingConfigKeyException final : public std::exception
	{
	public:
		explicit MissingConfigKeyException(std::string missingKey) noexcept : m_What("You're config file is not conform to the one available on github. It misses the key: " + std::move(missingKey)) {};

		[[nodiscard]] const char* what() const noexcept override
		{
			return m_What.data();
		}

	private:
		std::string m_What;
	};

	class EmptyConfigValueException final : public std::exception
	{
	public:
		explicit EmptyConfigValueException(std::string missingValueKey) noexcept : m_What("No value for the key: " + std::move(missingValueKey)) {};

		[[nodiscard]] const char* what() const noexcept override
		{
			return m_What.data();
		}

	private:
		std::string m_What;
	};

	Results Read();
};