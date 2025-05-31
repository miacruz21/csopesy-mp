#include "console.hpp"
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif

// Enable ANSI escape codes on Windows
void enable_virtual_terminal_processing() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

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

void Console::show_nvidia_smi_dummy() const {
    std::cout <<
R"(Fri Mar 29 18:42:38 2024
+-----------------------------------------------------------------------------------+
| NVIDIA-SMI 551.86             Driver Version: 551.86       CUDA Version: 12.4     |
|-------------------------------------+----------------------+----------------------+
| GPU  Name                 TCC/WDDM  | Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf       Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |
|                                     |                      | MIG M.               |
|=====================================+======================+======================|
|   0  Dummy GeForce GTX 1080    WDDM | 00000000:26:00.0  On |                  N/A |
| 28%   37C    P8          11W / 180W |     701MiB / 8192MiB |     0%       Default |
|                                     |                      |                  N/A |
+-------------------------------------+----------------------+----------------------+

+-----------------------------------------------------------------------------------+
| Processes:                                                                        |
|  GPU   GI   CI        PID   Type   Process name                        GPU Memory |
|        ID   ID                                                         Usage      |
|===================================================================================|
|    0   N/A  N/A      1234    C+G   C:\Windows\System32\dwm.exe           100MiB   |
|    0   N/A  N/A      2345    C+G   ...wekyb3d8bbwe\Xbox.exe              200MiB   |
|    0   N/A  N/A      3456    C+G   ...on\123.0.2420.65\msedge.exe        150MiB   |
|    0   N/A  N/A      4567    C+G   C:\Windows\explorer.exe               300MiB   |
|    0   N/A  N/A      5678    C+G   ...dws.wao23dwas022\SearchHost.exe    250MiB   |
+-----------------------------------------------------------------------------------+
)" << std::endl;
}

void Console::run() {
    enable_virtual_terminal_processing();
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
            else if (input == "nvidia-smi") {
                show_nvidia_smi_dummy();
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