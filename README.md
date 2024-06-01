# BetBot

A small bot discord to manage friendly bets on sports or esports matches. It was designed with League of Legend in mind, but it works with any competitive scene. 

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)

## Installation

### Prerequisites
- C++ 20 or higher
- D++ Library installed on your computer (https://dpp.dev/index.html)
- CMake 3.28 or higher

It should work on both Linux and Windows, but I'm only using the bot on a Linux server so I haven't tested it thoroughly on Windows. 

### Steps
1. Clone the repository
2. Create an output directory, go into it and run cmake with the following command: ```cmake <path-to-CMakeLists.txt>```. CMake will use the current directory as output directory, that's why you shouldn't forget to go into the output directory you created before running the command.
3. Compile. The command will depend on your system. On Linux, you should be able to do it by simply using ```make``` in the output directory. 
5. Update the config.json file. You must at least give a valid discord bot token.
6. Run. If you run directly from your output directory, you have nothing else to do since CMake should copy the config.json file in your output directory. If you moved the executable somewhere (on a server for example), don't forget to also move the config.json file next to it. 

## Usage

### add_match

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/8a8f023a-a422-4d26-9634-0fd1580dc19a)

Use it to add a match on which the users can bet. It has 3 parameters:

- The first two parameters are the names of the two teams
- The third parameter is the size of the 'Best Of'. A quick explanation in case you don't know what a "Best of X" is: in most esports and some traditional sports, a match between two teams is not limited to a single game. Instead, the teams will play a series of games called a "Best Of X" with X being the maximum number of games for the series. For example, a "Best Of 5" or "BO5" will have a maximum of 5 games and the first team to win 3 games wins the match. If you are betting on a traditional single match sport, you can simply set this parameter to "1".

### show_matches

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/dcb9d431-41c0-4c99-a921-ab00cff03624)

It will show you the existing matches like in the following image. Note that is also shows you the existing bets on the match.

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/512f6beb-cbbc-4e41-b4c3-780cbe8f9fe5)

It will also display a selector asking you what you want to do. You have different options:
- Do nothing if you simply wanted to see the list of incoming matches
- Select "Place a bet on a match" to bet on a match
- Select "Add a match result" once the match is over and you want to update the bettors' scores.

No matter what you select, the bot will answer with a new selector asking you to choose the match on which you want to operate:

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/e3d45ae7-fb48-41bc-92c7-53c55a706df5)

Then will come a new selector asking to choose a score (your bet or the final result). The options are automatically created depending on your Best Of size:

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/21232848-750e-44c6-8c47-7d637f8a1cdd)

Once selected, depending on what you chose to do at start, your bet will be placed or the result set and bettor's results updated.

### show_results

It allows you to see the global score of the different users. 

![image](https://github.com/BJPerez/DiscordBetBot/assets/16210134/dc49d6c1-5b55-4b4a-9b79-af8e8cdd4741)

A correct bet means that you had the correct winning team, it grants you 1 point.
A perfect bet means that you had to correct winning team and the exact score, it grants you < 1 + the size of the BO> points




 
