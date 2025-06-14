#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <vector>
#include <iostream>
#include "utils.hpp"
#include <fstream>
#include <mutex>

class ProcessScreen {
private:
    std::string name;
    std::time_t created_time;
    int current_instruction;
    int total_instructions;
    std::vector<std::string> logs;
    bool finished = false;
    int core_id = -1;
    std::ofstream print_file;
    bool enable_file_output = true;
    std::time_t finished_time;
    mutable std::mutex queue_mutex;
 
public:
    ProcessScreen(const std::string& name, int total_instructions = 10);
    void show_info() const;
    void interact();
    void add_log(const std::string& message);

    void print_to_file(const std::string& message) {
        if (!enable_file_output) return;
        
        if (!print_file.is_open()) {
            print_file.open(name + "_prints.txt", std::ios::app);
        }
        std::time_t now = std::time(nullptr);
        char time_str[100];
        std::strftime(time_str, sizeof(time_str), 
                    "(%m/%d/%Y %I:%M:%S%p)", std::localtime(&now));
        print_file << time_str << " Core:" << core_id << " \"" << message << "\"\n";
        print_file.flush();
    }

    void print_status() const {
        char time_str[100];
        std::strftime(time_str, sizeof(time_str), "%m/%d/%Y %I:%M:%S%p", 
                    std::localtime(&created_time));
        
        std::cout << std::setw(10) << std::left << name
                << "(" << time_str << ") "
                << "Core: " << std::setw(2) << core_id << "  "
                << std::setw(5) << current_instruction << " / " 
                << total_instructions;
                
        if (finished) {
            std::cout << " [FINISHED]";
        } else if (core_id == -1) {
            std::cout << " [WAITING]";
        } else {
            std::cout << " [RUNNING]";
        }
        std::cout << "\n";
    }

    std::time_t get_finished_time() const { return finished_time; }
    void set_finished_time() { 
        finished_time = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now()); 
    }

    void execute_next() {
        if (!finished && current_instruction < total_instructions) {
            current_instruction++;
            
            std::string msg = "Hello World from " + name + " (instruction " + 
                            std::to_string(current_instruction) + ")";
            print_to_file(msg);
            add_log(msg);
            
            if (current_instruction >= total_instructions) {
                finished = true;
                set_finished_time();
                std::string completion_msg = name + " completed all instructions";
                print_to_file(completion_msg);
                add_log(completion_msg);
            }
        }
    }

    void set_core(int id) { core_id = id; }
    int get_core() const { return core_id; }
    int get_current_instruction() const { return current_instruction; }
    int get_total_instructions() const { return total_instructions; }
    void set_current_instruction(int line) { current_instruction = line; }
    void mark_finished() { finished = true; }
    std::time_t get_created_time() const { return created_time; }

    bool is_finished() const { return finished; }
    const std::string& get_name() const { return name; }
};