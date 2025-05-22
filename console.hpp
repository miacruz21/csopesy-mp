#pragma once
#include <string>
#include <map>
#include "process.hpp"

class Console {
private:
    std::map<std::string, ProcessScreen> screens;
    bool initialized = false;

    void clear_screen() const;
    void handle_screen_command(const std::string& command);
    void print_header() const;
    void show_help() const;
    
public:
    void run();
};