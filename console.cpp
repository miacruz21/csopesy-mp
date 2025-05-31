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
    std::time_t now = std::time(nullptr);
    char time_str[100];
    std::strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", std::localtime(&now));
    
    std::cout << time_str << "\n\n";
    std::cout << "+---------------------------------------------------------------------------------------+\n";
    std::cout << "| NVIDIA-SMI 546.92                 Driver Version: 546.92       CUDA Version: 12.3     |\n";
    std::cout << "|-----------------------------------------+----------------------+----------------------+\n";
    std::cout << "| GPU  Name                     TCC/WDDM  | Bus-Id        Disp.A | Volatile Uncorr. ECC |\n";
    std::cout << "| Fan  Temp   Perf          Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |\n";
    std::cout << "|                                         |                      |               MIG M. |\n";
    std::cout << "|=========================================+======================+======================|\n";
    std::cout << "|   0  NVIDIA GeForce RTX 3060 ...  WDDM  | 00000000:01:00.0  On |                  N/A |\n";
    std::cout << "| N/A   44C    P5              20W /  95W |   1395MiB /  6144MiB |     19%      Default |\n";
    std::cout << "|                                         |                      |                  N/A |\n";
    std::cout << "+-----------------------------------------+----------------------+----------------------+\n\n";

    std::cout << "+---------------------------------------------------------------------------------------+\n";
    std::cout << "|  Processes:                                                                           |\n";
    std::cout << "|  GPU   GI   CI        PID   Type   Process name                            GPU Memory |\n";
    std::cout << "|        ID   ID                                                             Usage      |\n";
    std::cout << "+=======================================================================================+\n";

    struct Process {
        int pid;
        std::string type;
        std::string name;
        std::string memory;
    };
    
    std::vector<Process> processes = {
        {1368, "C+G", "C:\\Windows\\System32\\dwm.exe", "N/A"},
        {2116, "C+G", "C:\\Users\\User\\AppData\\Local\\Packages\\Microsoft.XboxGamingOverlay_8wekyb3d8bbwe\\xboxGameBarWidgets.exe", "N/A"},
        {4224, "C+G", "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", "N/A"},
        {5684, "C+G", "C:\\Windows\\explorer.exe", "N/A"},
        {6676, "C+G", "C:\\Windows\\System32\\DriverStore\\FileRepository\\nv_dispi.inf_amd64_neutral_hash\\Display.NvContainer\\NVDisplay.Container.exe", "N/A"},
        {6700, "C+G", "C:\\Windows\\System32\\DriverStore\\FileRepository\\nv_dispi.inf_amd64_neutral_hash\\Display.NvContainer\\NVDisplay.Container.exe", "N/A"}
    };

    // Display processes with proper alignment
    for (const auto& proc : processes) {
        std::string display_name = truncate_text(proc.name, 38);
        
        std::cout << "|    0   N/A  N/A    " 
                  << std::setw(6) << std::right << proc.pid << "   "
                  << std::setw(4) << std::right << proc.type << "   "
                  << std::setw(38) << std::left << display_name << "    "
                  << std::setw(3) << std::left << proc.memory << "      |\n";
    }
    std::cout << "+=======================================================================================+\n\n";
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