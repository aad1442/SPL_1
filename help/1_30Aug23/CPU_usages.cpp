#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>

// Structure to store CPU usage information
struct CpuUsageInfo {
    double prevIdle;
    double prevTotal;
    double usage;
};

// Function to calculate CPU usage
void calculateCpuUsage(CpuUsageInfo &cpuInfo)
{
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open())
    {
        std::cerr << "Failed to open /proc/stat\n";
        return;
    }

    std::string line;
    std::getline(statFile, line); // Read the first line (overall CPU stats)

    // Extract CPU usage values from the line
    long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    sscanf(line.c_str(), "cpu %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld",
           &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal, &guest, &guest_nice);

    // Calculate total CPU time
    double total = user + nice + system + idle + iowait + irq + softirq + steal;

    // Calculate CPU usage percentage
    double cpuUsage = 100.0 * (1.0 - ((idle - cpuInfo.prevIdle) / (total - cpuInfo.prevTotal)));

    // Update the CPU usage info structure
    cpuInfo.prevIdle = idle;
    cpuInfo.prevTotal = total;
    cpuInfo.usage = cpuUsage;
}

// Function to monitor CPU usage for a specified number of iterations
void monitorCpuUsage(int numIterations)
{
    CpuUsageInfo cpuInfo = {0.0, 0.0, 0.0};
    std::ofstream outFile("cpu_usage.txt",std::ios::app);

    if (!outFile.is_open())
    {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (int i = 0; i < numIterations; ++i)
    {
        calculateCpuUsage(cpuInfo);
        std::cout << "CPU Usage: " << cpuInfo.usage << "%\n";

        // Write CPU usage to the file
        outFile << "CPU Usage: " << cpuInfo.usage << "%" << std::endl;

        if (i < numIterations - 1)
        {
            // Wait for some time before calculating CPU usage again
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Calculate every 1 second
        }
    }

    outFile.close();
}

int main()
{
    int numIterations = 5; // Specify the number of iterations
    monitorCpuUsage(numIterations);
    return 0;
}
