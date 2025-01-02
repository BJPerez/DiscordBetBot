cmake_minimum_required(VERSION 3.25)

########################################
# Project setup
########################################
set(PROJECT_NAME BetBot)
project(${PROJECT_NAME} VERSION 2.1 DESCRIPTION "A simple discord bot to handle friendly bets.")

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

########################################
# Set up the compiler flags
########################################
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

########################################
# Source files
########################################
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

########################################
# Main -- separate executable
########################################
list(REMOVE_ITEM SRC_FILES ${PROJECT_SOURCE_DIR}/src/main.cpp)
# Key idea: SEPARATE OUT your main() function into its own file so it can be its
# own executable. Separating out main() means you can add this library to be
# used elsewhere (e.g linking to the test executable).

########################################
# Compile source files into a library
########################################
add_library(BetBot_Lib STATIC ${SRC_FILES})

set_target_properties(BetBot_Lib PROPERTIES
	CMAKE_CXX_FLAGS "-g -Wall"
)

########################################
# DPP LIB
########################################
add_subdirectory(./libs/DPP)

target_link_libraries(BetBot_Lib 
	dpp
)
	 
target_include_directories(BetBot_Lib PRIVATE
	libs/DPP/include
)
	 
########################################
# Main is separate (e.g. library client)
########################################
add_executable(BetBot ${PROJECT_SOURCE_DIR}/src/main.cpp)

########################################
# linking Main against the library
########################################
target_link_libraries(
	BetBot 
	BetBot_Lib
)
	 
########################################
# Copy config.json next to the main exe
########################################
add_custom_command(
		TARGET ${PROJECT_NAME}  POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				${CMAKE_SOURCE_DIR}/config.json
				${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json
)

########################################
# Testing
########################################
# Options. Turn on with 'cmake -Dmyvarname=ON'.
# option(BUILD_TESTS "Build all tests." ON) # Makes boolean 'test' available.

# google test is a git submodule for the project, and it is also cmake-based
add_subdirectory(./libs/googletest)

enable_testing()
	
# Include the gtest library. gtest_SOURCE_DIR is available due to
# 'project(gtest)' above.
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

########################################
# Test files
########################################
file(GLOB TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/*.cc)

########################################
# Unit Tests
########################################
add_executable(BotUnitTests ${TEST_SRC_FILES})

########################################
# Standard linking to gtest stuff.
########################################
target_link_libraries(BotUnitTests gtest gtest_main)

########################################
# Extra linking for the project.
########################################
target_link_libraries(BotUnitTests BetBot_Lib)
