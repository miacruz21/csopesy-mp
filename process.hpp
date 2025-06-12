#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <iostream>
#include "utils.hpp"

class ProcessScreen {
private:
    std::string name;
    std::time_t created_time;
    int current_instruction;
    int total_instructions;
    std::vector<std::string> logs;
    bool finished = false;
    int core_id = -1;
 
public:
    ProcessScreen(const std::string& name, int total_instructions = 10);
    void show_info() const;
    void interact();
    void add_log(const std::string& message);

    void print_status() const {
        char time_str[100];
        std::strftime(time_str, sizeof(time_str), "%m/%d/%y %I:%M:%S %p", std::localtime(&created_time));
        
        std::cout << std::setw(12) << std::left << name
                << "(" << time_str << ")   "
                << "Core: " << core_id << " "
                << current_instruction << " / " << total_instructions << "\n";
    }

    void execute_next() {
        if (!finished && current_instruction < total_instructions) {
            current_instruction++;
            add_log("Executed instruction " + std::to_string(current_instruction));
            if (current_instruction >= total_instructions) {
                finished = true;
            }
        }
    }

    void set_core(int id) { core_id = id; }
    int get_core() const { return core_id; }
    int get_current_instruction() const { return current_instruction; }
    int get_total_instructions() const { return total_instructions; }
    void set_current_instruction(int line) { current_instruction = line; }
    void mark_finished() { finished = true; }

    bool is_finished() const { return finished; }
    const std::string& get_name() const { return name; }
};