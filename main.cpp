#include "console.hpp"
#include "process.hpp"
#include <vector>
#include <unordered_map>
#include <random>

int generate_pid() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000, 9999);
    return dis(gen);
}

int main() {
    std::vector<ProcessScreen> processes;
    std::unordered_map<std::string, ProcessScreen*> process_map;

    processes.emplace_back("process_1", 100, generate_pid());
    processes.emplace_back("process_2", 100, generate_pid());
    processes.emplace_back("process_3", 100, generate_pid());

    // Assign cores and populate map
    for (size_t i = 0; i < processes.size(); ++i) {
        processes[i].set_core(static_cast<int>(i % 2)); // Assuming 2 cores
        process_map[processes[i].get_name()] = &processes[i];
    }

    // Simulate execution
    for (auto& proc : processes) {
        proc.execute_next(); // Execute one instruction
    }

    // --- System Status ---
    int used_cores = 0;
    for (const auto& proc : processes) {
        if (!proc.is_finished()) {
            used_cores++;
        }
    }
    int total_cores = 2; // Assuming 2 cores
    int utilization = (used_cores * 100) / total_cores;

    std::cout << "CPU Utilization: " << utilization << "%\n";
    std::cout << "Cores used: " << used_cores << "\n";
    std::cout << "Cores available: " << (total_cores - used_cores) << "\n\n";
    std::cout << "---------------------------------------------\n";
    
    // Show running processes
    std::cout << "Running processes:\n";
    for (const auto& proc : processes) {
        if (!proc.is_finished()) {
            proc.print_status();
        }
    }

    // Show finished processes
    std::cout << "\nFinished processes:\n";
    for (const auto& proc : processes) {
        if (proc.is_finished()) {
            proc.print_status();
        }
    }
    std::cout << "--------------------------------\n";

    // Demo O(1) access
    if (process_map.count("process_1")) {
        std::cout << "\nFound process_1 in O(1) time:\n";
        process_map["process_1"]->print_status();
    }

    Console console;
    console.run();

    return 0;
}