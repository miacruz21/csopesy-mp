#pragma once
#include "process.hpp"
#include <vector>
#include <unordered_map>
#include <memory>

class ProcessManager {
private:
    std::vector<std::unique_ptr<ProcessScreen>> processes;  // Owns processes
    std::unordered_map<std::string, ProcessScreen*> process_map;  // For O(1) access
    int total_cores = 2;
    int next_core = 0;

public:
    void add_process(const std::string& name, int total_instructions = 100) {
        auto new_process = std::make_unique<ProcessScreen>(name, total_instructions);
        auto* ptr = new_process.get();
        
        ptr->set_core(next_core);
        next_core = (next_core + 1) % total_cores;
        
        processes.push_back(std::move(new_process));
        process_map[name] = ptr;
    }

    ProcessScreen* get_process(const std::string& name) const {
        auto it = process_map.find(name);
        return it != process_map.end() ? it->second : nullptr;
    }

    void execute_all() {
        for (auto& proc : processes) {
            if (!proc->is_finished()) {
                proc->execute_next();
            }
        }
    }

    const std::vector<std::unique_ptr<ProcessScreen>>& get_all_processes() const {
        return processes;
    }

    std::vector<ProcessScreen*> get_running_processes() const {
        std::vector<ProcessScreen*> running;
        for (const auto& proc : processes) {
            if (!proc->is_finished()) {
                running.push_back(proc.get());
            }
        }
        return running;
    }

    void print_system_status() const {
        auto running = get_running_processes();
        int used_cores = std::min(static_cast<int>(running.size()), total_cores);
        
        std::cout << "CPU Utilization: " << (100 * used_cores / total_cores) << "%\n";
        std::cout << "Cores used: " << used_cores << "\n";
        std::cout << "Cores available: " << (total_cores - used_cores) << "\n\n";
        std::cout << "---------------------------------------------\n";
        
        std::cout << "Running processes:\n";
        for (auto* proc : running) {
            proc->print_status();
        }

        std::cout << "\nFinished processes:\n";
        for (const auto& proc : processes) {
            if (proc->is_finished()) {
                proc->print_status();
            }
        }
    }
};