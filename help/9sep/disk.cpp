#include <iostream>
#include <string>
#include <fstream>
#include <sys/statvfs.h>
#include <thread>
#include <chrono>
#include <vector>

struct SystemInfo {
    double totalSpaceGB;
    double availableSpaceGB;
    unsigned long f_bsize;  // File system block size
    unsigned long f_frsize; // Fundamental file system block size
    fsblkcnt_t f_blocks;    // Total number of blocks on file system
    fsblkcnt_t f_bfree;     // Total number of free blocks
    fsblkcnt_t f_bavail;    // Number of free blocks available to non-privileged process
    fsfilcnt_t f_files;     // Total number of file serial numbers
    fsfilcnt_t f_ffree;     // Total number of free file serial numbers
    fsfilcnt_t f_favail;    // Number of free file serial numbers available to non-privileged process
    unsigned long f_fsid;   // File system ID
    unsigned long f_flag;   // Bit mask of file system flags
    unsigned long f_namemax; // Maximum file name length
};

// Function to gather disk space and system information
SystemInfo gatherDiskAndSystemInfo(const std::string &path)
{
    struct statvfs vfs;
    SystemInfo info;

    if (statvfs(path.c_str(), &vfs) != 0)
    {
        std::cerr << "Failed to get disk and system information\n";
        return info; // Return an empty structure in case of failure
    }

    // Calculate disk space availability in GB
    unsigned long long totalSpace = vfs.f_frsize * vfs.f_blocks;
    unsigned long long availableSpace = vfs.f_frsize * vfs.f_bavail;
    info.totalSpaceGB = static_cast<double>(totalSpace) / (1024 * 1024 * 1024);
    info.availableSpaceGB = static_cast<double>(availableSpace) / (1024 * 1024 * 1024);

    // Populate the system data fields
    info.f_bsize = vfs.f_bsize;
    info.f_frsize = vfs.f_frsize;
    info.f_blocks = vfs.f_blocks;
    info.f_bfree = vfs.f_bfree;
    info.f_bavail = vfs.f_bavail;
    info.f_files = vfs.f_files;
    info.f_ffree = vfs.f_ffree;
    info.f_favail = vfs.f_favail;
    info.f_fsid = vfs.f_fsid;
    info.f_flag = vfs.f_flag;
    info.f_namemax = vfs.f_namemax;

    return info;
}

// Function to write data to a file
void writeDataToFile(const std::string &filePath, const std::vector<std::string> &data)
{
    std::ofstream outFile(filePath, std::ios::app); // Open in append mode
    if (!outFile)
    {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (const std::string &line : data) {
        outFile << line << std::endl;
    }

    outFile.close();
}

// Function to perform disk space and system information collection
void collectDiskAndSystemInfo(const std::string &diskPath, int numIterations)
{
    std::vector<std::string> collectedData;

    for (int i = 0; i < numIterations; ++i)
    {
        SystemInfo info = gatherDiskAndSystemInfo(diskPath);

        // Display and store disk space and system information
        collectedData.push_back("Disk and System Information:");
        collectedData.push_back("Disk Path: " + diskPath);
        collectedData.push_back("Total Disk Space: " + std::to_string(info.totalSpaceGB) + " GB");
        collectedData.push_back("Available Disk Space: " + std::to_string(info.availableSpaceGB) + " GB");
        collectedData.push_back("-----------------------");
        collectedData.push_back("System Data:");
        collectedData.push_back("File System Block Size: " + std::to_string(info.f_bsize));
        collectedData.push_back("Fundamental File System Block Size: " + std::to_string(info.f_frsize));
        collectedData.push_back("Total Number of Blocks on File System: " + std::to_string(info.f_blocks));
        collectedData.push_back("Total Number of Free Blocks: " + std::to_string(info.f_bfree));
        collectedData.push_back("Number of Free Blocks Available to Non-privileged Process: " + std::to_string(info.f_bavail));
        collectedData.push_back("Total Number of File Serial Numbers: " + std::to_string(info.f_files));
        collectedData.push_back("Total Number of Free File Serial Numbers: " + std::to_string(info.f_ffree));
        collectedData.push_back("Number of Free File Serial Numbers Available to Non-privileged Process: " + std::to_string(info.f_favail));
        collectedData.push_back("File System ID: " + std::to_string(info.f_fsid));
        collectedData.push_back("File System Flags: " + std::to_string(info.f_flag));
        collectedData.push_back("Maximum File Name Length: " + std::to_string(info.f_namemax));
        collectedData.push_back("-----------------------");

        // Delay for 2 seconds before the next calculation
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Specify the file path for writing
    std::string filePath = "disk_space_info.txt";

    // Write collected data to the file
    writeDataToFile(filePath, collectedData);

    std::cout << "Disk space and system information written to " << filePath << std::endl;
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

    // Perform disk space and system information collection
    collectDiskAndSystemInfo(diskPath, numIterations);

    return 0;
}
