#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <ctime>

struct SystemMetrics {
    double totalSpaceGB;
    double availableSpaceGB;
    unsigned long f_bsize;
    unsigned long f_frsize;
    fsblkcnt_t f_blocks;
    fsblkcnt_t f_bfree;
    fsblkcnt_t f_bavail;
    fsfilcnt_t f_files;
    fsfilcnt_t f_ffree;
    fsfilcnt_t f_favail;
    unsigned long f_fsid;
    unsigned long f_flag;
    unsigned long f_namemax;
    long uptime;
    double loadAverage[3];
    time_t currentTime;
    struct tm* localTime;
};

// Function to gather disk space availability
SystemMetrics gatherSystemMetrics(const std::string& path)
{
    struct statvfs vfs;
    struct sysinfo si;
    SystemMetrics metrics;

    if (statvfs(path.c_str(), &vfs) != 0) {
        std::cerr << "Failed to get disk space information\n";
        return metrics; // Return an empty structure in case of failure
    }

    // Calculate disk space availability in GB
    unsigned long long totalSpace = vfs.f_frsize * vfs.f_blocks;
    unsigned long long availableSpace = vfs.f_frsize * vfs.f_bavail;
    metrics.totalSpaceGB = static_cast<double>(totalSpace) / (1024 * 1024 * 1024);
    metrics.availableSpaceGB = static_cast<double>(availableSpace) / (1024 * 1024 * 1024);

    // Populate the rest of the data structure with system metrics
    metrics.f_bsize = vfs.f_bsize;
    metrics.f_frsize = vfs.f_frsize;
    metrics.f_blocks = vfs.f_blocks;
    metrics.f_bfree = vfs.f_bfree;
    metrics.f_bavail = vfs.f_bavail;
    metrics.f_files = vfs.f_files;
    metrics.f_ffree = vfs.f_ffree;
    metrics.f_favail = vfs.f_favail;
    metrics.f_fsid = vfs.f_fsid;
    metrics.f_flag = vfs.f_flag;
    metrics.f_namemax = vfs.f_namemax;

    // Get system uptime
    metrics.uptime = sysinfo(&si) ? -1 : si.uptime;

    // Get load average
    if (getloadavg(metrics.loadAverage, 3) == -1) {
        metrics.loadAverage[0] = metrics.loadAverage[1] = metrics.loadAverage[2] = -1;
    }

    // Get current time
    metrics.currentTime = std::time(nullptr);
    metrics.localTime = std::localtime(&metrics.currentTime);

    return metrics;
}

// Function to write metrics to a file
void writeMetricsToFile(const std::string& filePath, const SystemMetrics& metrics)
{
    std::ofstream outFile(filePath, std::ios::app); // Open in append mode

    if (!outFile) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    outFile << "System Metrics:\n";
    outFile << "----------------\n";
    outFile << "Total Disk Space: " << metrics.totalSpaceGB << " GB\n";
    outFile << "Available Disk Space: " << metrics.availableSpaceGB << " GB\n";
    outFile << "Disk Block Size: " << metrics.f_bsize << " bytes\n";
    outFile << "File System Block Size: " << metrics.f_frsize << " bytes\n";
    outFile << "Total Blocks: " << metrics.f_blocks << "\n";
    outFile << "Free Blocks in File System: " << metrics.f_bfree << "\n";
    outFile << "Free Blocks Available to Non-Superuser: " << metrics.f_bavail << "\n";
    outFile << "Total Inodes: " << metrics.f_files << "\n";
    outFile << "Free Inodes: " << metrics.f_ffree << "\n";
    outFile << "Free Inodes Available to Non-Superuser: " << metrics.f_favail << "\n";
    outFile << "File System ID: " << metrics.f_fsid << "\n";
    outFile << "Mount Flags: " << metrics.f_flag << "\n";
    outFile << "Maximum File Name Length: " << metrics.f_namemax << "\n";
    outFile << "System Uptime: " << metrics.uptime << " seconds\n";
    outFile << "Load Average (1 minute): " << metrics.loadAverage[0] << "\n";
    outFile << "Load Average (5 minutes): " << metrics.loadAverage[1] << "\n";
    outFile << "Load Average (15 minutes): " << metrics.loadAverage[2] << "\n";
    outFile << "Current Local Time: " << asctime(metrics.localTime);

    std::cout << "System metrics written to " << filePath << std::endl;

    outFile.close();
}

int main()
{
    std::string diskPath;

    // Prompt the user for the disk path with example options
    // Root directory: /
    // Home directory for a user: /home/username
    // Mounted external drive: /media/external_drive
    std::cout << "Enter the disk path (e.g., '/'): ";
    std::cin >> diskPath;

    // Specify the number of iterations
    int numIterations = 5;

    // Specify the output file path
    std::string filePath = "system_metrics.txt"; // Fixed file path

    for (int i = 0; i < numIterations; ++i) {
        SystemMetrics metrics = gatherSystemMetrics(diskPath);
        writeMetricsToFile(filePath, metrics);

        // Delay for 2 seconds before the next iteration
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
