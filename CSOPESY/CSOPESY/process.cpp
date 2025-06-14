#include "Process.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>

// Constructor
Process::Process(std::string name, int pid, int burst, int arrival)
    : name(std::move(name)), pid(pid), burstTime(burst), arrivalTick(arrival), finishTick(-1), finished(false) {

	// .txt log file per process (P1 == P1.txt, P2 == P2.txt, etc.)
    std::string filename = this->name + ".txt";
    log_file.open(filename, std::ios::out | std::ios::app);
    if (log_file.is_open()) {
        log_file << "Process name: " << this->name << "\nlogs:\n\n";
    }
}

// Move constructor: Moving ng process sa ibang mem location
Process::Process(Process&& other) noexcept
    : name(std::move(other.name)),
      pid(other.pid),
      burstTime(other.burstTime),
      arrivalTick(other.arrivalTick),
      finishTick(other.finishTick),
      finished(other.finished),
      log_file(std::move(other.log_file)) {
}

// getting formatted na curremtTime 
std::string Process::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();       //current time
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm;
#ifdef _WIN32
    localtime_s(&tm, &t); // for Windows
#else
    localtime_r(&t, &tm); //pag Linux/Mac
#endif
    std::ostringstream oss;
    oss << "(" << std::put_time(&tm, "%d/%m/%Y %I:%M:%S%p") << ")";
    return oss.str(); // formatted na string ng curtime 
}

// running ng process based sa burstTime
void Process::run(int& globalTick) {
    int core = 0; // change for more cores
    for (int i = 0; i < burstTime; ++i) {
        ++globalTick;  // incremet one tick per loop
        print(core);   //logs the output
    }
    //when its finished
	finishTick = globalTick; 
    finished = true;
}

// "Hello world"  message with time and core ID sa log
void Process::print(int core) {
    if (log_file.is_open()) {
        log_file << getCurrentTimestamp()
            << " Core: " << core
            << " \"Hello world from " << name << "!\"\n";
    }
}

// getters for info ng process
int Process::getFinishTick() const { return finishTick; }
int Process::getArrivalTick() const { return arrivalTick; }
int Process::getBurstTime() const { return burstTime; }
int Process::getPID() const { return pid; }
std::string Process::getName() const { return name; }

// Move assignment operator: transfers data ng isang process to another 
Process& Process::operator=(Process&& other) noexcept {
    if (this != &other) {
        name = std::move(other.name);
        pid = other.pid;
        burstTime = other.burstTime;
        arrivalTick = other.arrivalTick;
        finishTick = other.finishTick;
        finished = other.finished;
        log_file = std::move(other.log_file);
    }
    return *this;
}
