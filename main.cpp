#include <iostream>
#include <string>
#include <algorithm>
#include <cstdlib>

void clearScreen(const std::string& header) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::cout << header << "\n\n";
}

int main() {
    const std::string COLOR_GREEN = "\033[32m";
    const std::string COLOR_LIGHT_YELLOW = "\033[93m";
    const std::string COLOR_RESET = "\033[0m";
    const std::string header = 
        R"(
   _____  _____  ____  _____  ______  _______     __
  / ____|/ ____|/ __ \|  __ \|  ____|/ ____\ \   / /
 | |    | (___ | |  | | |__) | |__  | (___  \ \_/ / 
 | |     \___ \| |  | |  ___/|  __|  \___ \  \   /  
 | |____ ____) | |__| | |    | |____ ____) |  | |   
  \_____|_____/ \____/|_|    |______|_____/   |_|   
)"
        + std::string(COLOR_GREEN) + "Hello, Welcome to CSOPESY commandline!\n" + COLOR_RESET
        + COLOR_LIGHT_YELLOW + "Type 'exit' to quit, 'clear' to clear the screen\n" + COLOR_RESET;

    std::cout << header << "\n";
    
    bool initialized = false;
    std::string input;

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        std::transform(input.begin(), input.end(), input.begin(), 
                      [](unsigned char c){ return std::tolower(c); });

        if (input == "exit") {
            std::exit(0);
        }

        if (!initialized && input != "initialize") {
            std::cout << "Error: You must initialize first with 'initialize' command.\n";
            continue;
        }

        if (input == "initialize") {
            std::cout << "initialize command recognized. Initializing system...\n";
            initialized = true;
        }
        else if (input == "clear") {
            clearScreen(header);
        }
        else if (input == "screen") {
            std::cout << "screen command recognized. Doing something...\n";
        }
        else if (input == "scheduler-start") {
            std::cout << "scheduler-start command recognized. Doing something...\n";
        }
        else if (input == "scheduler-stop") {
            std::cout << "scheduler-stop command recognized. Doing something...\n";
        }
        else if (input == "report-util") {
            std::cout << "report-util command recognized. Doing something...\n";
        }
        else {
            std::cout << "Error: Unrecognized command. Available commands: initialize, screen, "
                      << "scheduler-start, scheduler-stop, report-util, clear, exit\n";
        }
    }

    return 0;
}