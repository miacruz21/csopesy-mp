#include "console.hpp"
#include <iostream>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif

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
              << "  screen -s <name> - Create new process screen\n"
              << "  screen -r <name> - Attach to process screen\n"
              << "  screen -ls - List all processes\n"
              << "  scheduler-start - Start scheduler\n"
              << "  scheduler-stop - Stop scheduler\n"
              << "  report-util - Generate utilization report\n"
              << "  clear - Refresh console\n"
              << "  exit - Quit program\n";
}

void Console::handle_screen_command(const std::string& command) {
    if (command == "screen -ls") {
        std::cout << "\n";
        if (process_manager) {
            process_manager->print_system_status();
        } else {
            std::cout << "Process manager not initialized\n";
        }
    }
    else if (command.rfind("screen -s ", 0) == 0) {
        std::string name = command.substr(10);
        if (!process_manager) {
            std::cout << "Process manager not initialized\n";
            return;
        }
        
        if (process_manager->get_process(name)) {
            std::cout << "Error: Process '" << name << "' already exists\n";
        } else {
            // Create new process with 100 instructions
            process_manager->add_process(name, 100);
            std::cout << "Created process '" << name << "' with 100 instructions\n";
            
            // Enter the process screen
            clear_screen();
            ProcessScreen* process = process_manager->get_process(name);
            process->interact();
            clear_screen();
        }
    }
    else if (command.rfind("screen -r ", 0) == 0) {
        std::string name = command.substr(10);
        if (!process_manager) {
            std::cout << "Process manager not initialized\n";
            return;
        }
        
        ProcessScreen* process = process_manager->get_process(name);
        if (process) {
            if (!process->is_finished()) {
                clear_screen();
                process->interact();
                clear_screen();
            } else {
                std::cout << "Error: Process '" << name << "' has finished\n";
            }
        } else {
            std::cout << "Error: Process '" << name << "' not found\n";
        }
    }
    else {
        std::cout << "Invalid screen command\n";
    }
}

// void Console::show_nvidia_smi_dummy() const {
//     std::time_t now = std::time(nullptr);
//     char time_str[100];
//     std::strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", std::localtime(&now));
    
//     std::cout << time_str << "\n\n";
//     std::cout << "+---------------------------------------------------------------------------------------+\n";
//     std::cout << "| NVIDIA-SMI 546.92                 Driver Version: 546.92       CUDA Version: 12.3     |\n";
//     std::cout << "|-----------------------------------------+----------------------+----------------------+\n";
//     std::cout << "| GPU  Name                     TCC/WDDM  | Bus-Id        Disp.A | Volatile Uncorr. ECC |\n";
//     std::cout << "| Fan  Temp   Perf          Pwr:Usage/Cap |         Memory-Usage | GPU-Util  Compute M. |\n";
//     std::cout << "|                                         |                      |               MIG M. |\n";
//     std::cout << "|=========================================+======================+======================|\n";
//     std::cout << "|   0  NVIDIA GeForce RTX 3060 ...  WDDM  | 00000000:01:00.0  On |                  N/A |\n";
//     std::cout << "| N/A   44C    P5              20W /  95W |   1395MiB /  6144MiB |     19%      Default |\n";
//     std::cout << "|                                         |                      |                  N/A |\n";
//     std::cout << "+-----------------------------------------+----------------------+----------------------+\n\n";

//     std::cout << "+---------------------------------------------------------------------------------------+\n";
//     std::cout << "|  Processes:                                                                           |\n";
//     std::cout << "|  GPU   GI   CI        PID   Type   Process name                            GPU Memory |\n";
//     std::cout << "|        ID   ID                                                             Usage      |\n";
//     std::cout << "+=======================================================================================+\n";

//     struct Process {
//         int pid;
//         std::string type;
//         std::string name;
//         std::string memory;
//     };
    
//     std::vector<Process> processes = {
//         {1368, "C+G", "C:\\Windows\\System32\\dwm.exe", "N/A"},
//         {2116, "C+G", "C:\\Users\\User\\AppData\\Local\\Packages\\Microsoft.XboxGamingOverlay_8wekyb3d8bbwe\\xboxGameBarWidgets.exe", "N/A"},
//         {4224, "C+G", "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", "N/A"},
//         {5684, "C+G", "C:\\Windows\\explorer.exe", "N/A"},
//         {6676, "C+G", "C:\\Windows\\System32\\DriverStore\\FileRepository\\nv_dispi.inf_amd64_neutral_hash\\Display.NvContainer\\NVDisplay.Container.exe", "N/A"},
//         {6700, "C+G", "C:\\Windows\\System32\\DriverStore\\FileRepository\\nv_dispi.inf_amd64_neutral_hash\\Display.NvContainer\\NVDisplay.Container.exe", "N/A"}
//     };

//     for (const auto& proc : processes) {
//         std::string display_name = truncate_text(proc.name, 38);
        
//         std::cout << "|    0   N/A  N/A    " 
//                   << std::setw(6) << std::right << proc.pid << "   "
//                   << std::setw(4) << std::right << proc.type << "   "
//                   << std::setw(38) << std::left << display_name << "    "
//                   << std::setw(3) << std::left << proc.memory << "      |\n";
//     }
//     std::cout << "+=======================================================================================+\n\n";
// }

void Console::show_nvidia_smi_dummy() const {
    std::time_t now = std::time(nullptr);
    char time_str[100];
    std::strftime(time_str, sizeof(time_str), "%a %b %d %H:%M:%S %Y", std::localtime(&now));
    
    std::cout << "+---------------------------------------------------------------------------------------+\n";
    std::cout << "| " << std::setw(20) << std::left << truncate_text("NVIDIA-SMI 546.92", 20)
              << " " << std::setw(22) << std::left << truncate_text("Driver Version: 546.92", 22)
              << " " << std::setw(20) << std::left << truncate_text("CUDA Version: 12.3", 20) << " |\n";
    std::cout << "|-----------------------------------------+----------------------+----------------------+\n";

    // GPU Status Header
    std::cout << "| " << std::setw(30) << std::left << "GPU  Name"
              << " " << std::setw(8) << std::left << "TCC/WDDM"
              << " | " << std::setw(18) << std::left << "Bus-Id"
              << " | " << std::setw(18) << std::left << "Volatile Uncorr. ECC" << " |\n";

    // GPU Status Values (with truncation)
    std::cout << "| " << std::setw(30) << std::left << truncate_text("NVIDIA GeForce RTX 3060", 30)
              << " " << std::setw(8) << std::left << truncate_text("WDDM", 8)
              << " | " << std::setw(18) << std::left << truncate_text("00000000:01:00.0", 18)
              << " | " << std::setw(18) << std::left << truncate_text("N/A", 18) << " |\n";

    // GPU Utilization Header
    std::cout << "| " << std::setw(3) << std::left << "Fan"
              << " " << std::setw(4) << std::left << "Temp"
              << " " << std::setw(6) << std::left << "Perf"
              << " " << std::setw(14) << std::left << "Pwr:Usage/Cap"
              << " | " << std::setw(18) << std::left << "Memory-Usage"
              << " | " << std::setw(18) << std::left << "GPU-Util" << " |\n";

    // GPU Utilization Values (with truncation)
    std::cout << "| " << std::setw(3) << std::left << truncate_text("N/A", 3)
              << " " << std::setw(4) << std::left << truncate_text("43C", 4)
              << " " << std::setw(6) << std::left << truncate_text("P5", 6)
              << " " << std::setw(14) << std::left << truncate_text("21W / 95W", 14)
              << " | " << std::setw(18) << std::left << truncate_text("1308MiB / 6144MiB", 18)
              << " | " << std::setw(18) << std::left << truncate_text("20%", 18) << " |\n";
    std::cout << "|                                         |                      |                      |\n";
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
        {1368, "C+G", "C:\\Windows\\System32\\dwm.exe", "1395MiB"},
        {2116, "C+G", "C:\\Users\\User\\AppData\\Local\\Packages\\Microsoft.XboxGamingOverlay_8wekyb3d8bbwe\\xboxGameBarWidgets.exe", "422MiB"},
        {4224, "C+G", "C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe", "785MiB"},
        {5684, "C+G", "C:\\Windows\\explorer.exe", "210MiB"},
        {6676, "C+G", "C:\\Windows\\System32\\DriverStore\\FileRepository\\nv_dispi.inf_amd64_neutral_hash\\Display.NvContainer\\NVDisplay.Container.exe", "85MiB"}
    };

    for (const auto& proc : processes) {
        std::cout << "|    0   N/A  N/A    " 
                  << std::setw(6) << std::right << proc.pid << "   "
                  << std::setw(4) << std::right << truncate_text(proc.type, 4) << "   "
                  << std::setw(38) << std::left << truncate_text(proc.name, 38) << "    "
                  << std::setw(8) << std::left << truncate_text(proc.memory, 8) << " |\n";
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
                initialized = true;
                
                for (int i = 1; i <= 10; ++i) {
                    std::string name = "process_" + std::to_string(i);
                    process_manager->add_process(name, 100);
                }
                process_manager->print_system_status();
                process_manager->start_scheduler();
                std::cout << "Scheduler started (FCFS with 4 cores)\n";

            } else {
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
                std::cout << "report-util command recognized. Generating report...\n";
                // Implementation would go here
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