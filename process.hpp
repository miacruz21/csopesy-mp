#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <iostream>

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

    // Add: Print process status in the required format
    void print_status() const {
        std::tm* tm_ptr = std::localtime(&created_time);
        std::cout << name << "    ("
                  << std::put_time(tm_ptr, "%m/%d/%y %H:%M:%S") << ")"
                  << "  Core: " << core_id
                  << " " << current_instruction << " / " << total_instructions
                  << std::endl;
    }

    // Add: Setters and getters for new attributes
    void set_core(int id) { core_id = id; }
    int get_core() const { return core_id; }
    int get_current_instruction() const { return current_instruction; }
    int get_total_instructions() const { return total_instructions; }
    void set_current_instruction(int line) { current_instruction = line; }
    void mark_finished() { finished = true; }

    bool is_finished() const { return finished; }
    const std::string& get_name() const { return name; }
};