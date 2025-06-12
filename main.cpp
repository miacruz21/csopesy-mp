#include "console.hpp"
#include "process.hpp"
#include "process_manager.hpp"
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
    ProcessManager manager;

    manager.add_process("process_1");
    manager.add_process("process_2");
    manager.add_process("process_3");

    // Execute some steps
    for (int i = 0; i < 5; i++) {
        manager.execute_all();
    }

    manager.print_system_status();

    // Demo O(1) access
    if (auto proc = manager.get_process("process_1")) {
        std::cout << "\nFound process_1 in O(1) time:\n";
        proc->print_status();
    }

    Console console;
    console.run();
    return 0;
}