#include "console.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>
#include <windows.h>

void clearScreen() {
    system("cls");
}

void moveCursor(int x, int y) {
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void runMarquee() {
    std::string text = " [Hello World in marquee!] ";

    //console size
    int width = 80, height = 25;

    int x = 0, y = 0;
    int dx = 1, dy = 1;

    int refreshDelay = 100; // ms
    int pollDelay = 40;

    clearScreen();


    while (true) {

        clearScreen();
        std::cout << "\n ******************************" << std::endl;
        std::cout << "\n Displaying a marquee console! " << std::endl;
        std::cout << "\n ******************************" << std::endl;
        moveCursor(x, y);
        std::cout << text << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(refreshDelay));

        // diagonal
        x += dx;
        y += dy;

        // horizontal
        if (x <= 0 || x + (int)text.length() >= width) dx *= -1;
        // vertical
        if (y <= 0 || y >= height - 1) dy *= -1;

        // Checks input
        for (int i = 0; i < refreshDelay / pollDelay; ++i) {
            if (_kbhit()) {
                (void)_getch(); //ignores the return value 
                return; // Exit marquee on any key press
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(pollDelay));
        }
    }
}
