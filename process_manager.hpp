#pragma once
#include "process.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <condition_variable>

class ProcessManager {
private:
    std::vector<std::unique_ptr<ProcessScreen>> processes;
    std::unordered_map<std::string, ProcessScreen*> process_map;
    int total_cores = 2;
    int next_core = 0;
    std::vector<std::thread> worker_threads;
    bool running = false;
    std::condition_variable cv;
    bool stop_requested = false;
    std::mutex execution_mutex;
    std::unordered_map<std::string, bool> process_busy_map;
    mutable std::mutex queue_mutex;

public:
    explicit ProcessManager(int cores = 4) : total_cores(cores) {}

    void worker_thread(int core_id) {
        while (running) {
            std::unique_ptr<ProcessScreen> process;
            
            {
                std::unique_lock<std::mutex> lock(queue_mutex);
                cv.wait(lock, [this] { 
                    return stop_requested || 
                        std::any_of(processes.begin(), processes.end(),
                            [this](auto& p) { 
                                return !p->is_finished() && !process_busy_map[p->get_name()]; 
                            });
                });
                
                if (stop_requested) return;
                
                // Find oldest unfinished process NOT already being processed
                auto it = std::min_element(processes.begin(), processes.end(),
                    [this](auto& a, auto& b) {
                        bool a_available = !a->is_finished() && !process_busy_map[a->get_name()];
                        bool b_available = !b->is_finished() && !process_busy_map[b->get_name()];
                        
                        if (!a_available) return false;
                        if (!b_available) return true;
                        return a->get_created_time() < b->get_created_time();
                    });
                
                if (it != processes.end() && !(*it)->is_finished()) {
                    process = std::move(*it);
                    processes.erase(it);
                    process->set_core(core_id);
                    process_busy_map[process->get_name()] = true;
                }
            }
            
            if (process) {
                // Execute ALL instructions for this process
                while (!process->is_finished()) {
                    process->execute_next();
                    std::this_thread::sleep_for(std::chrono::milliseconds(50));
                }
                
                {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    process_busy_map[process->get_name()] = false;
                    processes.push_back(std::move(process));
                }
                cv.notify_all();
            }
        }
    }

    void start_scheduler() {
        running = true;
        stop_requested = false;
        
        // Single scheduler thread
        worker_threads.emplace_back([this] { scheduler_thread(); });
        
        // Worker threads (one per core)
        for (int i = 0; i < total_cores; ++i) {
            worker_threads.emplace_back([this, i] { worker_thread(i); });
        }
    }
    
    void scheduler_thread() {
        while (running && !all_processes_completed()) {
            std::unique_lock<std::mutex> lock(queue_mutex);
            cv.notify_all(); 
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void stop_scheduler() {
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            running = false;
            stop_requested = true;
        }
        cv.notify_all();
        for (auto& thread : worker_threads) {
            if (thread.joinable()) thread.join();
        }
        worker_threads.clear();
    }

    bool all_processes_completed() {
        std::lock_guard<std::mutex> lock(queue_mutex);
        return std::all_of(processes.begin(), processes.end(),
            [](auto& p) { return p->is_finished(); });
    }

    void add_process(const std::string& name, int total_instructions = 100) {
        auto new_process = std::make_unique<ProcessScreen>(name, total_instructions);
        auto* ptr = new_process.get();
        
        {
            std::lock_guard<std::mutex> lock(queue_mutex);
            processes.push_back(std::move(new_process));
            process_map[name] = ptr;
            process_busy_map[name] = false;
        }
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
        std::lock_guard<std::mutex> lock(queue_mutex);
    
        // Combine all processes (both in vector and being executed)
        std::vector<ProcessScreen*> all_processes;
        
        // 1. Add processes from the main vector
        for (const auto& proc : processes) {
            all_processes.push_back(proc.get());
        }
        
        // 2. Add processes currently being executed (from process_busy_map)
        for (const auto& [name, busy] : process_busy_map) {
            if (busy) {
                auto* proc = process_map.at(name);
                all_processes.push_back(proc);
            }
        }
        
        // Print header
        std::cout << "\nSystem Status (" << all_processes.size() << " processes)\n";
        std::cout << "=================================================================\n";
        std::cout << std::setw(12) << std::left << "Process"
                << std::setw(22) << "Time"
                << std::setw(8) << "Core"
                << std::setw(15) << "Progress"
                << "Status\n";
        std::cout << "-----------------------------------------------------------------\n";
        
        // Print all processes
        for (auto* proc : all_processes) {
            char time_str[100];
            std::time_t time = proc->is_finished() ? proc->get_finished_time() : proc->get_created_time();
            std::strftime(time_str, sizeof(time_str), "%m/%d/%Y %I:%M:%S%p", 
                        std::localtime(&time));
            
            std::cout << std::setw(12) << std::left << proc->get_name()
                    << std::setw(22) << time_str
                    << std::setw(8) << (proc->get_core() == -1 ? "-" : std::to_string(proc->get_core()))
                    << std::setw(7) << proc->get_current_instruction() << " / " 
                    << std::setw(7) << proc->get_total_instructions();
            
            if (proc->is_finished()) {
                std::cout << " [FINISHED]";
            } else if (proc->get_core() == -1) {
                std::cout << " [WAITING]";
            } else {
                std::cout << " [RUNNING]";
            }
            std::cout << "\n";
        }
        std::cout << "=================================================================\n\n";
    }
};