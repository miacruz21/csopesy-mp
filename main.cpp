#include "console.hpp"
#include "process.hpp"
#include "process_manager.hpp"
#include <vector>
#include <unordered_map>
#include <random>

int main() {
    ProcessManager manager;
    Console console;
    console.set_process_manager(&manager);
    console.run();
    manager.stop_scheduler();
    return 0;
}