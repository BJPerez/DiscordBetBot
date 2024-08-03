	# Minimum CMake version required, we'll just use the latest version.
	cmake_minimum_required(VERSION 3.25)
	# Project name, version and description
	project(BetBot VERSION 2.0 DESCRIPTION "A simple discord bot to handle friendly bets.")
	 
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
	 
	# Create an executable
	add_executable(${PROJECT_NAME}
	    "src/main.cpp"
		"src/AddBetCommand.cpp"
		"src/AddBetCommand.h"
		"src/AddMatchCommand.cpp"
		"src/AddMatchCommand.h"
		"src/AddResultCommand.cpp"
		"src/AddResultCommand.h"
		"src/Bet.h"
		"src/BettorResults.cpp"
		"src/BettorResults.h"
		"src/CommandBase.h"
		"src/ICommandReceiver.cpp"
		"src/ICommandReceiver.h"
		"src/BotData.cpp"
		"src/BotData.h"
		"src/Match.cpp"
		"src/Match.h"
		"src/MatchScore.cpp"
		"src/MatchScore.h"
		"src/ShowBettorsResultsCommand.cpp"
		"src/ShowBettorsResultsCommand.h"
		"src/ShowMatchesCommand.cpp"
		"src/ShowMatchesCommand.h"
		"src/JsonSerializer.cpp"
		"src/JsonSerializer.h"
		"src/SaveManager.cpp"
		"src/SaveManager.h"
		"src/BetBot.cpp"
		"src/BetBot.h"
		"src/DrawUtils.cpp"
		"src/DrawUtils.h"
		"src/ShowBetProposalCommand.cpp"
		"src/ShowBetProposalCommand.h"
		"src/ChooseMatchToBetOnCommand.cpp"
		"src/ChooseMatchToBetOnCommand.h"
		"src/ChooseMatchToSetResultCommand.cpp"
		"src/ChooseMatchToSetResultCommand.h"
		"src/ShowResultProposalCommand.cpp"
		"src/ShowResultProposalCommand.h"
		"src/FileWatcher.cpp"
		"src/FileWatcher.h"
		"src/LockableDataAccessors.h"
		"src/BotConfigReader.cpp"
		"src/BotConfigReader.h"
		)
	 
	# Find our pre-installed DPP package (using FindDPP.cmake).
	find_package(DPP REQUIRED)
	 
	# Link the pre-installed DPP package.
	target_link_libraries(${PROJECT_NAME} 
	    ${DPP_LIBRARIES}
	)
	 
	# Include the DPP directories.
	target_include_directories(${PROJECT_NAME} PRIVATE
	    ${DPP_INCLUDE_DIR}
	)
	 
	# Set C++ version
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    CXX_STANDARD 20
	    CXX_STANDARD_REQUIRED ON
	)

	add_custom_command(
			TARGET ${PROJECT_NAME}  POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_SOURCE_DIR}/config.json
					${CMAKE_CURRENT_BINARY_DIR}/config.json
	)