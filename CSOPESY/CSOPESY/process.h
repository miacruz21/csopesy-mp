#pragma once
#include <string>
#include <fstream>

class Process {
private:
    std::string name;
    int pid;
    int burstTime;
    int arrivalTick;
    int finishTick;
    bool finished = false; // Initialize to false
    std::ofstream log_file;

public:
    Process(std::string name, int pid, int burst, int arrival);

    // move constructor declaration with noexcept
    Process(Process&& other) noexcept;
    //move assignment operator declaration with noexcept
    Process& operator=(Process&& other) noexcept;

    void run(int& globalTick); // Simulation
    int getFinishTick() const;
    int getArrivalTick() const;
    int getBurstTime() const;
    int getPID() const;
    std::string getName() const;

    void print(int core);
    static std::string getCurrentTimestamp();
};
