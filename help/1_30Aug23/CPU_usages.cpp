#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <vector>

// Structure to store CPU usage information
struct CpuUsageInfo {
    double user; //the cpu is running kernel code
    double nice;  //The cpu is running in user-mode
    double system; //Nice is when the CPU is executing a user task having below-normal priority
    double idle;   //when the runnable queue is empty and there are no io operation going the cpu usage is marked as idle
    double iowait; //io wait is when the cpu is waiting for an io operation to complete , and the cpu cannotbe used for anything else
    double irq; //the kernel is servicing interrupt request
    double softirq;
    double steal;// when running in a virtualized environment, the hypervisor may steal cycles that are meant for your cpus and givr them to another, for various reason
    double guest;
    double guest_nice;

//     user – time spent in user mode
// nice – time spent processing nice processes in user mode
// system – time spent executing kernel code
// idle – time spent idle
// iowait – time spent waiting for I/O
// irq – time spent servicing interrupts
// softirq – time spent servicing software interrupts
// steal – time stolen from a virtual machine
// guest – time spent running a virtual CPU for a guest operating system
// guest_nice – time spent running a virtual CPU for a “niced” guest operating system
};

// Function to parse CPU statistics from a line in /proc/stat
bool parseCpuStats(const std::string& line, CpuUsageInfo& cpuInfo) {
    return sscanf(line.c_str(), "cpu %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
        &cpuInfo.user, &cpuInfo.nice, &cpuInfo.system, &cpuInfo.idle,
        &cpuInfo.iowait, &cpuInfo.irq, &cpuInfo.softirq, &cpuInfo.steal,
        &cpuInfo.guest, &cpuInfo.guest_nice) == 10;
}

// Function to calculate total CPU time
double calculateTotalCpuTime(const CpuUsageInfo& cpuInfo) {
    return cpuInfo.user + cpuInfo.nice + cpuInfo.system + cpuInfo.idle +
           cpuInfo.iowait + cpuInfo.irq + cpuInfo.softirq + cpuInfo.steal +
           cpuInfo.guest + cpuInfo.guest_nice;
}

// Function to calculate CPU usage percentage
double calculateCpuUsagePercentage(const CpuUsageInfo& current, const CpuUsageInfo& previous) {
    double currentTotal = calculateTotalCpuTime(current);
    double previousTotal = calculateTotalCpuTime(previous);
    double currentIdle = current.idle;
    double previousIdle = previous.idle;

    double cpuUsage = 100.0 * (1.0 - ((currentIdle - previousIdle) / (currentTotal - previousTotal)));
    //This calculation provides insight into how actively the CPU is being used between the two measurement points.
    return cpuUsage;
}

// Function to calculate CPU usage
void calculateCpuUsage(CpuUsageInfo& cpuInfo) {
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) {
        std::cerr << "Failed to open /proc/stat\n";
        return;
    }

    std::string line;
    std::getline(statFile, line); // Read the first line (overall CPU stats)

    if (!parseCpuStats(line, cpuInfo)) {
        std::cerr << "Failed to parse CPU statistics\n";
        return;
    }
}

// Function to monitor CPU usage for a specified number of iterations
void monitorCpuUsage(int numIterations) {
    std::vector<CpuUsageInfo> cpuStats;
    cpuStats.reserve(numIterations);

    CpuUsageInfo cpuInfo;
    CpuUsageInfo previousCpuInfo;

    for (int i = 0; i < numIterations; ++i) {
        calculateCpuUsage(cpuInfo);
        cpuStats.push_back(cpuInfo);

        double cpuUsage = 0.0;

        if (i > 0) {
            cpuUsage = calculateCpuUsagePercentage(cpuInfo, previousCpuInfo);
            std::cout << "Iteration " << i << ": CPU Usage - " << cpuUsage << "%\n";
        }

        // Update previous CPU info
        previousCpuInfo = cpuInfo;

        if (i < numIterations - 1) {
            // Wait for some time before calculating CPU usage again
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Calculate every 1 second
        }
    }

    // Write CPU usage statistics to a file
    std::ofstream outFile("cpu_usage.txt");

    if (!outFile.is_open()) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (const CpuUsageInfo& info : cpuStats) {
        outFile << "User: " << info.user << "%, Nice: " << info.nice << "%, System: " << info.system
                << "%, Idle: " << info.idle << "%, IOWait: " << info.iowait << "%, IRQ: " << info.irq
                << "%, SoftIRQ: " << info.softirq << "%, Steal: " << info.steal << "%, Guest: " << info.guest
                << "%, Guest Nice: " << info.guest_nice << "%" << std::endl;
    }

    outFile.close();
}

int main() {
    int numIterations = 5; // Specify the number of iterations
    monitorCpuUsage(numIterations);
    return 0;
}
