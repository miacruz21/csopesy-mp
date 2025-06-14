#include "process.hpp"
#include <iostream>

ProcessScreen::ProcessScreen(const std::string& name, int total_instructions) 
    : name(name), total_instructions(total_instructions), current_instruction(0) 
{
    created_time = std::chrono::system_clock::to_time_t(
        std::chrono::system_clock::now());
    // Initialize the output file immediately
    if (enable_file_output) {
        print_file.open(name + "_prints.txt");
        print_file << "Process " << name << " created at: " 
                 << std::ctime(&created_time) << "\n";
        print_file.close();  // Will reopen in append mode when needed
    }
    add_log("Process created");
}

void ProcessScreen::add_log(const std::string& message) {
    char time_str[100];
    std::strftime(time_str, sizeof(time_str), 
                 "(%m/%d/%Y %I:%M:%S%p)", std::localtime(&created_time));
    
    std::string core_str = "Core:" + std::to_string(core_id);
    logs.push_back(std::string(time_str) + " " + core_str + " \"" + message + "\"");
}

void ProcessScreen::show_info() const {
    char time_str[100];
    std::strftime(time_str, sizeof(time_str), 
                 "%m/%d/%Y, %I:%M:%S %p", std::localtime(&created_time));

    std::cout << "\n=== Process Screen ===\n"
              << "Process name: " << name << "\n"
              << "Instruction: " << current_instruction 
              << "/" << total_instructions << "\n"
              << "Created: " << time_str << "\n";

    for (const auto& log : logs) {
        std::cout << log << "\n";
    }
}

void ProcessScreen::interact() {
    show_info();
    std::string input;
    
    while (true) {
        std::cout << "[" << name << "]> ";
        std::getline(std::cin, input);

        if (input == "exit") break;
        
        if (input == "process-smi") {
            show_info();
            std::cout << "Logs:\n";
            for (const auto& log : logs) {
                std::cout << log << "\n";
            }
            if (finished) std::cout << "Finished!\n";
        }
        else if (input.rfind("print ", 0) == 0) {
            std::string message = input.substr(6);
            add_log("Printed: " + message);
            print_to_file(message);
            std::cout << "Message printed to file\n";
            
            // Count this as an instruction
            if (!finished) {
                current_instruction++;
                if (current_instruction >= total_instructions) {
                    finished = true;
                    add_log("Process completed");
                }
            }
        }
        else if (!finished) {
            current_instruction++;
            std::string msg = "Executed instruction " + std::to_string(current_instruction);
            add_log(msg);
            std::cout << msg << "\n";
            
            if (current_instruction >= total_instructions) {
                finished = true;
                add_log("Process completed");
            }
        }
        else {
            std::cout << "Process has finished. Type 'exit' to return to main console\n";
        }
    }
}