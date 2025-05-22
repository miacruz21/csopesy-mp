#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>

class ProcessScreen {
private:
    std::string name;
    std::time_t created_time;
    int current_instruction;
    int total_instructions;
    std::vector<std::string> logs;
    bool finished = false;
    
public:
    ProcessScreen(const std::string& name, int total_instructions = 10);
    void show_info() const;
    void interact();
    void add_log(const std::string& message);
    bool is_finished() const { return finished; }
    const std::string& get_name() const { return name; }
};