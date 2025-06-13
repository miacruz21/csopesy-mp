#include "scheduler.hpp"
#include <iostream>
#include <sstream>

Process::Process(const std::string& name, int id)
    : name(name), id(id), finished(false) {
    std::string filename = name + ".txt";
    log_file.open(filename);
    log_file << "Process name: " << name << "\nLogs:\n\n";
}

void Process::execute(int core_id) {
    for (int i = 1; i <= 100; i++) {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time);
        
        std::ostringstream oss;
        oss << "(" << std::put_time(&tm, "%m/%d/%Y %I:%M:%S%p")
            << ") Core:" << core_id << " \"Hello world from " << name << "!\"";
        
        log_file << oss.str() << "\n";
        
        // Simulate work
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    finished = true;
}

bool Process::is_finished() const {
    return finished;
}

std::string Process::get_name() const {
    return name;
}

int Process::get_id() const {
    return id;
}

CPU::CPU(int id, int& active_processes)
    : id(id), active_processes(active_processes), stop_flag(false) {
    thread = std::thread(&CPU::run, this);
}

void CPU::run() {
    while (!stop_flag) {
        Process* process = nullptr;
        
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this] { return !process_queue.empty() || stop_flag; });
            
            if (stop_flag) return;
            
            process = process_queue.front();
            process_queue.pop();
        }
        
        if (process) {
            process->execute(id);
            active_processes--;
        }
    }
}

void CPU::assign_process(Process* process) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        process_queue.push(process);
    }
    cv.notify_one();
}

bool CPU::is_busy() const {
    return !process_queue.empty();
}

void CPU::stop() {
    stop_flag = true;
    cv.notify_one();
    if (thread.joinable()) {
        thread.join();
    }
}

Scheduler::Scheduler(int num_cores)
    : running(false), active_processes(0) {
    for (int i = 0; i < num_cores; i++) {
        cores.emplace_back(i, active_processes);
    }
}

Scheduler::~Scheduler() {
    stop();
    for (auto process : all_processes) {
        delete process;
    }
}

void Scheduler::start() {
    running = true;
    scheduler_thread = std::thread(&Scheduler::schedule, this);
}

void Scheduler::stop() {
    running = false;
    cv.notify_one();
    if (scheduler_thread.joinable()) {
        scheduler_thread.join();
    }
    
    for (auto& core : cores) {
        core.stop();
    }
}

void Scheduler::add_process(Process* process) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        process_queue.push(process);
        all_processes.push_back(process);
        active_processes++;
    }
    cv.notify_one();
}

void Scheduler::schedule() {
    while (running || active_processes > 0) {
        Process* process = nullptr;
        
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this] { 
                return !process_queue.empty() || !running; 
            });
            
            if (!running && process_queue.empty()) break;
            
            process = process_queue.front();
            process_queue.pop();
        }
        
        if (process) {
            bool assigned = false;
            for (auto& core : cores) {
                if (!core.is_busy()) {
                    core.assign_process(process);
                    assigned = true;
                    break;
                }
            }
            
            if (!assigned) {
                std::lock_guard<std::mutex> lock(mutex);
                process_queue.push(process);
            }
        }
    }
}

std::vector<std::pair<std::string, bool>> Scheduler::get_processes_status() const {
    std::vector<std::pair<std::string, bool>> status;
    for (auto process : all_processes) {
        status.emplace_back(process->get_name(), process->is_finished());
    }
    return status;
}