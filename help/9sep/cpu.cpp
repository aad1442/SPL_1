#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
std::vector<double> monitorCpuUsage(int numIterations) {
    std::vector<double> cpuUsageHistory;
    cpuUsageHistory.reserve(numIterations);

    CpuUsageInfo cpuInfo;
    CpuUsageInfo previousCpuInfo;

    for (int i = 0; i < numIterations; ++i) {
        calculateCpuUsage(cpuInfo);

        double cpuUsage = 0.0;

        if (i > 0) {
            cpuUsage = calculateCpuUsagePercentage(cpuInfo, previousCpuInfo);
        }

        // Store CPU usage in history
        cpuUsageHistory.push_back(cpuUsage);

        // Update previous CPU info
        previousCpuInfo = cpuInfo;

        if (i < numIterations - 1) {
            // Sleep for 1 second before the next iteration
            sleep(1);
        }
    }

    return cpuUsageHistory;
}

// Function to log CPU usage results to a file
void logCpuUsage(const std::vector<double>& cpuUsageHistory) {
    std::ofstream outFile("cpu_usage.txt");

    if (!outFile.is_open()) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (int i = 0; i < cpuUsageHistory.size(); ++i) {
        outFile << "Iteration " << i + 1 << ": CPU Usage - " << cpuUsageHistory[i] << "%" << std::endl;
    }

    outFile.close();

    std::cout << "CPU usage statistics written to cpu_usage.txt\n";
}

int main() {
    int numIterations = 5; // Specify the number of iterations
    std::vector<double> cpuUsageHistory = monitorCpuUsage(numIterations);

    // Log CPU usage results to a file
    logCpuUsage(cpuUsageHistory);

    return 0;
}
