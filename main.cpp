#include "console.hpp"
#include "process.hpp"
#include <vector>
#include <unordered_map>

int main() {
    // Store processes for linear traversal
    std::vector<ProcessScreen> processes;
    processes.emplace_back("process_1", 100);
    processes.emplace_back("process_2", 100);

     // Assign core_id based on index
    for (size_t i = 0; i < processes.size(); ++i) {
        processes[i].set_core(static_cast<int>(i));
    }

    // Store processes for O(1) retrieval
    std::unordered_map<std::string, ProcessScreen*> process_map;
    for (auto& proc : processes) {
        process_map[proc.get_name()] = &proc;
    }

    // --- for the summary and section headers ---
    std::cout << "CPU Utilization: 100%\n";
    std::cout << "Cores used: 2\n";
    std::cout << "Cores available: 0\n\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Running processes:\n";
    // ---------------------------------------------------------

    // Traverse all processes
    for (const auto& proc : processes) {
        proc.print_status();
    }

     std::cout << "\nFinished processes:\n";
    std::cout << "---------------------------------------------\n";

    // Retrieve a process in O(1)
    if (process_map.count("process_1")) {
        process_map["process_1"]->print_status();
        std::cout << "Process 'process_1' found in O(1) time.\n";
    }
    
    Console console;
    console.run();
    return 0;
}