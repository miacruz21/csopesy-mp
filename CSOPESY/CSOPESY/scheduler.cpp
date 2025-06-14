#include "Scheduler.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

// Constructor:resets all initial values
Scheduler::Scheduler()
    : globalTick(0), pidCounter(0), stopRequested(false) {
}

// adds dummy process sa queue
void Scheduler::addDummyProcesses(int count) {
    for (int i = 1; i <= count; ++i) {
        auto p = std::make_unique<Process>("P" + std::to_string(i), pidCounter++, 3 + i, globalTick);
        RQ.push(std::move(p)); //  moves unique pointer sa queue
    }
}

// runs FCFS scheduling while may process pa
void Scheduler::run() {
    int core = 0;
    while (!RQ.empty() && !stopRequested) {
        std::unique_ptr<Process> current = std::move(RQ.front()); // gets next process 
        RQ.pop();

        
        for (int i = 0; i < 100 && !stopRequested; ++i) {
           current->print(core); //logs message
            std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Delay para may pacing
            //current->run(globalTick);
            globalTick++; // Tumataas ang tick per print
			//core = 0; //FCFS uses only one core at a time 
            //(core + 1) % numCores; // distributed sa iba't ibang core 
        }

		// added sa finished list after processing
        finishedQueue.push_back(std::move(current));
    }
}

// for shceduler stop function
void Scheduler::stop() {
    stopRequested = true;
}

// generates .txt file for the CPU utilization report
void Scheduler::reportUtil() {
    std::ofstream log("csopesy-log.txt", std::ios::out);
    if (!log.is_open()) {
        std::cerr << "[system message]: Cannot open file.\n";
        return;
    }

    // I-log ang lahat ng natatapos na process
    log << "Tick Count: " << globalTick << "\n";
    log << "Finished Processes:\n";
    for (const auto& p : finishedQueue) {
        log << p->getName() << " (PID " << p->getPID() << ") | Arrived: "
            << p->getArrivalTick() << ", Finished: " << p->getFinishTick()
            << ", Burst: " << p->getBurstTime() << "\n";
    }
    std::cout << "Report saved in csopesy-log.txt\n";
}

// checks if may nakapila pa sa RQ (ready queue)
bool Scheduler::isRunning() const {
    return !RQ.empty();
}

// search for a process by name inside finishedQueue
Process* Scheduler::findProcessByName(const std::string& name) {
    //TODO:
    for (auto& procPtr : finishedQueue) {
        if (procPtr->getName() == name) return procPtr.get();
    }
    return nullptr;
}

// to allow manually add ng process sa queue
void Scheduler::addProcess(Process process) {
    RQ.push(std::make_unique<Process>(std::move(process)));
}

// displays all processes in finished queue
std::vector<std::string> Scheduler::getFinishedProcessNames() const {
    std::vector<std::string> names;
    for (const auto& procPtr : finishedQueue) {
        names.push_back(procPtr->getName());
    }
    return names;
}
