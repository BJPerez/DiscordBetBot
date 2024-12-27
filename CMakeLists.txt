	cmake_minimum_required(VERSION 3.25)
	project(BetBot VERSION 2.1 DESCRIPTION "A simple discord bot to handle friendly bets.")

	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})  

	# ------------------------ Common -----------------------------
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
	list(APPEND PROJECT_MAIN_TESTED_SRC  
		"src/MatchScore.cpp"
		"src/MatchScore.h"
	)
	list(APPEND PROJECT_TEST_SRC  
		"tests/MatchScore_Tests.cc"
	)
	 
	# ------------------------ Main Executable -----------------------------
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
		"src/CommandBase.cpp"
		"src/CommandBase.h"
		"src/ICommandReceiver.cpp"
		"src/ICommandReceiver.h"
		"src/BotData.cpp"
		"src/BotData.h"
		"src/BotDataExceptions.h"
		"src/Match.cpp"
		"src/Match.h"
		"src/ShowBettorsResultsCommand.cpp"
		"src/ShowBettorsResultsCommand.h"
		"src/ShowIncomingMatchesCommand.cpp"
		"src/ShowIncomingMatchesCommand.h"
		"src/JsonSerializer.cpp"
		"src/JsonSerializer.h"
		"src/SaveManager.cpp"
		"src/SaveManager.h"
		"src/BetBot.cpp"
		"src/BetBot.h"
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
		"src/MessageBuilder.cpp"
		"src/MessageBuilder.h"
		${PROJECT_MAIN_TESTED_SRC}
		)

	# Add DPP as dependency
	add_subdirectory(libs/DPP)
	 
	# Link the pre-installed DPP package.
	target_link_libraries(${PROJECT_NAME} 
	    dpp
	)
	 
	# Include the DPP directories.
	target_include_directories(${PROJECT_NAME} PRIVATE
	    libs/DPP/include
	)
	 
	set_target_properties(${PROJECT_NAME} PROPERTIES
	    CXX_STANDARD 20
	    CXX_STANDARD_REQUIRED ON
	)

	# Copy config.json next to the exe
	add_custom_command(
			TARGET ${PROJECT_NAME}  POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
					${CMAKE_SOURCE_DIR}/config.json
					${CMAKE_CURRENT_BINARY_DIR}/config.json
	)

	# ------------------------ Test executable -----------------------------
	# Google Test
	include(FetchContent)
	FetchContent_Declare(
	  googletest
	  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
	)

	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)

	enable_testing()

	add_executable(Tests
		${PROJECT_MAIN_TESTED_SRC}
		${PROJECT_TEST_SRC}
	)
	target_link_libraries(Tests GTest::gtest_main)
	target_compile_definitions(Tests PRIVATE TEST_ENABLED=1)

	set_target_properties(Tests PROPERTIES
	    CXX_STANDARD 20
	    CXX_STANDARD_REQUIRED ON
	)