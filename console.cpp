#include "console.hpp"
#include <iostream>
#include <algorithm>

const std::string COLOR_GREEN = "\033[32m";
const std::string COLOR_LIGHT_YELLOW = "\033[93m";
const std::string COLOR_RESET = "\033[0m";
const std::string HEADER = R"(
   _____  _____  ____  _____  ______  _______     __
  / ____|/ ____|/ __ \|  __ \|  ____|/ ____\ \   / /
 | |    | (___ | |  | | |__) | |__  | (___  \ \_/ / 
 | |     \___ \| |  | |  ___/|  __|  \___ \  \   /  
 | |____ ____) | |__| | |    | |____ ____) |  | |   
  \_____|_____/ \____/|_|    |______|_____/   |_|   
)"
+ COLOR_GREEN + "\nWelcome to CSOPESY commandline!\n" + COLOR_RESET
+ COLOR_LIGHT_YELLOW + "Type 'initialize' to begin. 'exit' to quit, 'clear' to refresh\n" + COLOR_RESET;

void Console::clear_screen() const {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    std::cout << HEADER << "\n";
}

void Console::print_header() const {
    std::cout << HEADER << "\n";
}

void Console::show_help() const {
    std::cout << "Available commands:\n"
              << "  initialize - Initialize system\n"
              << "  screen -s <name> - Create new screen\n"
              << "  screen -r <name> - Attach to screen\n"
              << "  screen -ls - List screens\n"
              << "  scheduler-start - Start process generation\n"
              << "  scheduler-stop - Stop process generation\n"
              << "  report-util - Generate utilization report\n"
              << "  clear - Refresh console\n"
              << "  exit - Quit program\n";
}

void Console::handle_screen_command(const std::string& command) {
    if (command == "screen -ls") {
        std::cout << "\nActive Screens:\n";
        for (const auto& [name, screen] : screens) {
            std::cout << "- " << name << " (" 
                      << (screen.is_finished() ? "finished" : "running") << ")\n";
        }
    }
    else if (command.rfind("screen -s ", 0) == 0) {
        std::string name = command.substr(10);
        if (screens.count(name) == 0) {
            screens.emplace(std::piecewise_construct,
                          std::forward_as_tuple(name),
                          std::forward_as_tuple(name));
            clear_screen();
            screens.at(name).interact();
            clear_screen();
        } else {
            std::cout << "Error: Screen '" << name << "' already exists\n";
        }
    }
    else if (command.rfind("screen -r ", 0) == 0) {
        std::string name = command.substr(10);
        if (screens.count(name)) {
            if (!screens.at(name).is_finished()) {
                clear_screen();
                screens.at(name).interact();
                clear_screen();
            } else {
                std::cout << "Error: Process '" << name << "' has finished\n";
            }
        } else {
            std::cout << "Error: Screen '" << name << "' not found\n";
        }
    }
    else {
        std::cout << "Invalid screen command\n";
    }
}

void Console::run() {
    print_header();
    
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        if (input == "exit") {
            std::cout << "exit command recognized. Exiting...\n";
            break;
        }
        else if (input == "clear") {
            std::cout << "clear command recognized. Refreshing console...\n";
            clear_screen();
        }
        else if (!initialized) {
            if (input == "initialize") {
                std::cout << "initialize command recognized. Initializing system...\n";
                initialized = true;
            }
            else {
                std::cout << "Error: You must initialize first with 'initialize' command\n";
            }
        }
        else {
            if (input == "scheduler-start") {
                std::cout << "scheduler-start command recognized. Doing something...\n";
            }
            else if (input == "scheduler-stop") {
                std::cout << "scheduler-stop command recognized. Doing something...\n";
            }
            else if (input == "report-util") {
                std::cout << "report-util command recognized. Doing something...\n";
            }
            else if (input.rfind("screen", 0) == 0) {
                handle_screen_command(input);
            }
            else {
                std::cout << "Error: Unrecognized command\n";
                show_help();
            }
        }
    }
}