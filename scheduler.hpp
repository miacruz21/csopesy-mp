#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "process.hpp"

class Process {
public:
    Process(const std::string& name, int id);
    void execute(int core_id);
    bool is_finished() const;
    std::string get_name() const;
    int get_id() const;

private:
    std::string name;
    int id;
    std::atomic<bool> finished;
    std::ofstream log_file;
};

class CPU {
public:
    CPU(int id, int& active_processes);
    void assign_process(Process* process);
    bool is_busy() const;
    void stop();

private:
    void run();
    int id;
    int& active_processes;
    std::thread thread;
    std::queue<Process*> process_queue;
    std::mutex mutex;
    std::condition_variable cv;
    std::atomic<bool> stop_flag;
};

class Scheduler {
public:
    Scheduler(int num_cores = 4);
    ~Scheduler();
    void start();
    void stop();
    void add_process(Process* process);
    std::vector<std::pair<std::string, bool>> get_processes_status() const;

private:
    void schedule();
    std::vector<CPU> cores;
    std::queue<Process*> process_queue;
    std::vector<Process*> all_processes;
    std::thread scheduler_thread;
    std::atomic<bool> running;
    std::mutex mutex;
    std::condition_variable cv;
    int active_processes;
};