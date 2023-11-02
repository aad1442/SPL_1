#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <unistd.h> // for sleep function

// Structure to store CPU usage information
struct CpuUsageInfo {
    double user;      // Time spent in user mode
    double nice;      // Time spent in user mode with low priority (nice)
    double system;    // Time spent running kernel code
    double idle;      // Time spent idle
    double iowait;    // Time spent waiting for I/O
    double irq;       // Time spent servicing hardware interrupts (IRQ)
    double softirq;   // Time spent servicing software interrupts (softirq)
    double steal;     // Time stolen from a virtual machine (VM)
    double guest;     // Time spent running a virtual CPU for a guest OS
    double guest_nice;// Time spent running a "niced" guest OS
    double cpuUsage;  // Calculated CPU usage percentage
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

    // Calculate the total CPU time used between measurements
    double deltaTotal = currentTotal - previousTotal;

    // Calculate the CPU time used for computation (excluding idle time) between measurements
    double deltaUsage = deltaTotal - (current.idle - previous.idle);

    // Calculate the CPU usage percentage
    double cpuUsage = 100.0 * (deltaUsage / deltaTotal);
    return cpuUsage;
}

// Function to calculate CPU usage
void calculateCpuUsage(CpuUsageInfo& cpuInfo, const CpuUsageInfo& previousCpuInfo) {
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

    // Calculate CPU usage and store it in the structure
    cpuInfo.cpuUsage = calculateCpuUsagePercentage(cpuInfo, previousCpuInfo);
}

// Function to monitor CPU usage for a specified number of iterations
std::vector<CpuUsageInfo> monitorCpuUsage(int numIterations) {
    std::vector<CpuUsageInfo> cpuStats;
    cpuStats.reserve(numIterations);

    CpuUsageInfo cpuInfo;
    CpuUsageInfo previousCpuInfo;

    for (int i = 0; i < numIterations; ++i) {
        calculateCpuUsage(cpuInfo, previousCpuInfo);

        // Store CPU usage info in the vector
        cpuStats.push_back(cpuInfo);

        // Update previous CPU info
        previousCpuInfo = cpuInfo;

        if (i < numIterations - 1) {
            // Sleep for 1 second before the next iteration
            sleep(1);
        }
    }

    return cpuStats;
}

// Function to log CPU usage results to a file
void logCpuUsage(const std::vector<CpuUsageInfo>& cpuStats) {
    std::ofstream outFile("cpu_usage.txt");

    if (!outFile.is_open()) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    outFile << "Iteration    User           Nice      System       Idle          IOWait          IRQ        SoftIRQ      Steal        Guest        Guest Nice   CPU Usage (%)" << std::endl;

    for (int i = 0; i < cpuStats.size(); ++i) {
        const CpuUsageInfo& info = cpuStats[i];
        outFile << std::setw(5) << i + 1 << std::setw(13) << info.user << std::setw(13) << info.nice
                << std::setw(13) << info.system << std::setw(13) << info.idle << std::setw(13) << info.iowait
                << std::setw(13) << info.irq << std::setw(13) << info.softirq << std::setw(13) << info.steal
                << std::setw(13) << info.guest << std::setw(13) << info.guest_nice << std::setw(17) << info.cpuUsage << std::endl;
    }

    outFile.close();

    std::cout << "CPU usage statistics written to cpu_usage.txt\n";
}


int main() {
    int numIterations = 30; // Specify the number of iterations
    std::vector<CpuUsageInfo> cpuStats = monitorCpuUsage(numIterations);

    // Log CPU usage results to a file
    logCpuUsage(cpuStats);

    return 0;
}
