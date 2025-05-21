#include <iostream>
#include <string>
#ifdef _WIN32
#include <cstdlib> // For system("cls")
#else
#include <unistd.h>
#endif

using namespace std;

// Function to display the ASCII Header
void printHeader() {
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

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printHeader();
}

int main() {
    string command;
    printHeader();

    while (true) {
        cout << "Enter command: ";
        getline(cin, command);

        if (command == "initialize") {
            cout << "initialize command recognized. Doing something." << endl;
        }
        else if (command == "screen") {
            cout << "screen command recognized. Doing something." << endl;
        }
        else if (command == "scheduler-test") {
            cout << "scheduler-test command recognized. Doing something." << endl;
        }
        else if (command == "scheduler-stop") {
            cout << "scheduler-stop command recognized. Doing something." << endl;
        }
        else if (command == "report-util") {
            cout << "report-util command recognized. Doing something." << endl;
        }
        else if (command == "clear") {
            clearScreen();
        }
        else if (command == "exit") {
            cout << "Exiting the program." << endl;
            break;
        }
        else {
            cout << "Unknown command. Please try again." << endl;
        }
    }

    return 0;
}