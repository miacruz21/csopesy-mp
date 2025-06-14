#include "Scheduler.h"
#include <iostream>
#include <string>
#include "process.h"
#include "console.h"
#ifdef _WIN32
#include <cstdlib> // For system("cls")
#else
#include <unistd.h>
#endif
#include <thread>
#include <chrono>

using namespace std;

// Function to display the ASCII Header
static void printHeader() {
    const std::string COLOR_GREEN = "\033[32m";
    const std::string COLOR_YELLOW = "\033[93m";
    const std::string COLOR_RESET = "\033[0m";

    cout << COLOR_RESET;
    cout << R"( 
  _____  _____  ____  _____  ______  _______     __
 / ____|/ ____|/ __ \|  __ \|  ____|/ ____\ \   / /
| |    | (___ | |  | | |__) | |__  | (___  \ \_/ / 
| |     \___ \| |  | |  ___/|  __|  \___ \  \   /  
| |____ ____) | |__| | |    | |____ ____) |  | |   
 \_____|_____/ \____/|_|    |______|_____/   |_|   
)";
    cout << "\n ==========================" << endl;
    cout << "\n Hello, Welcome to CSOPESY commandline!\n" << COLOR_GREEN;
    cout << "\n Developers: \n Abergos, Mark Gabriel  \n Arca, Althea Denisse\n Cruz,Mia Bernice \n Del Mundo, Rahmon Khayle !\n" << COLOR_RESET;
    cout << "\n Last Updated: 05-16-2025 \n" << COLOR_RESET;
    cout << "\n ==========================\n " << endl;
    cout << COLOR_YELLOW << "Type 'exit' to quit, 'clear' to clear the screen\n" << COLOR_YELLOW;
    cout << COLOR_RESET;
}

static void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printHeader();
}

static void processScreen(Process& proc) {
    clearScreen();
    std::string input;
    while (true) {
        std::cout << "process> ";
        std::getline(std::cin, input);
        if (input == "process-smi") {
            std::cout << "Name: " << proc.getName() << "\n";
            std::cout << "PID: " << proc.getPID() << "\n";
            std::cout << "Burst: " << proc.getBurstTime() << "\n";
            std::cout << "Arrived: " << proc.getArrivalTick() << "\n";
            std::cout << "Finished: " << (proc.getFinishTick() > 0 ? std::to_string(proc.getFinishTick()) : "Running") << "\n";
            // TODO: Print dummy logs 
            if (proc.getFinishTick() > 0) {
                std::cout << "Finished!\n";
            }
        } else if (input == "exit") {
            break;
        } else {
            std::cout << "Unknown command.\n";
        }
    }
}

int main() {
    Scheduler scheduler;
	scheduler.addDummyProcesses(10); // HW6 - Add 10 dummy processes for testing

    // Run scheduler in a separate thread
    std::thread schedulerThread([&scheduler]() { scheduler.run(); });

    string command;
    printHeader();

    while (true) {
        cout << "Enter command: ";
        getline(cin, command);

        if (command == "initialize") {
            cout << "initialize command recognized. Doing something." << endl;
        }
        //TODO: pag start mag sscreen -s 10 times
        else if (command.rfind("screen -s ", 0) == 0) { // starts with "screen -s "
			std::string procName = command.substr(10); // get process name example" [screen -s P1] 
            // Find process by name (implement findProcessByName)
            Process* proc = scheduler.findProcessByName(procName);
            if (proc) {
                processScreen(*proc);
            } else {
				std::cout << "[System message]: Process not found.\n"; // or already finished
            }
        }
		else if (command == "screen -ls") { // Lists all processes
            auto finished = scheduler.getFinishedProcessNames();
            if (finished.empty()) {
                std::cout << "No finished processes  yet.\n";
            } else {
                std::cout << "Finished processes:\n";
                for (const auto& name : finished) {
                    std::cout << "  " << name << "\n";
                }
            }
        }
        else if (command == "scheduler-test") {
            //scheduler.addDummyProcesses(3);
            scheduler.run();
        }
        else if (command == "scheduler-stop") {
            scheduler.stop();
            cout << "Scheduler stopped." << endl;
        }
        else if (command == "report-util") {
            scheduler.reportUtil();
        }
        else if (command == "clear") {
            clearScreen();
        }
        else if (command == "exit") {
            cout << "Exiting the program." << endl;
            scheduler.stop(); // Signal the scheduler to stop
            if (schedulerThread.joinable()) {
                schedulerThread.join(); // Waits for the scheduler thread to finish
            }
            break;
        }
        else if (command == "marquee") {

            cout << "\n ******************************" << endl;
            cout << "\n Displaying a marquee console! " << endl;
            cout << "\n ******************************" << endl;

            runMarquee();
        }
        else {
            cout << "Unknown command. Please try again." << endl;
        }
    }

    return 0;
}