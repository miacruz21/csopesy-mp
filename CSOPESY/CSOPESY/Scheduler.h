#pragma once

#include <queue>
#include <vector>
#include <memory>
#include <atomic>
#include "process.h"

// First Come First Serve (FCFS) Scheduler 
class Scheduler {
public:
    Scheduler();
    void addDummyProcesses(int count);
    void run();
    void reportUtil();
    bool isRunning() const;
    Process* findProcessByName(const std::string& name);
    void addProcess(Process process); 
    std::vector<std::string> getFinishedProcessNames() const;
    void stop(); 

private:
    std::queue<std::unique_ptr<Process>> RQ;
    std::vector<std::unique_ptr<Process>> finishedQueue;
    int globalTick;
    int pidCounter;
    int numCores = 4; // 4 CPU cores
    std::atomic<bool> stopRequested{false}; 
};
