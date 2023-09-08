#include <iostream>
#include <string>
#include <fstream>
#include <sys/statvfs.h>
#include <thread>
#include <chrono>

struct DiskSpaceInfo {
    double totalSpaceGB;
    double availableSpaceGB;
};

// Function to gather disk space availability
DiskSpaceInfo gatherDiskSpaceAvailability(const std::string &path)
{
    struct statvfs vfs;
    DiskSpaceInfo info;

    if (statvfs(path.c_str(), &vfs) != 0)
    {
        std::cerr << "Failed to get disk space information\n";
        return info; // Return an empty structure in case of failure
    }

    // Calculate disk space availability in GB
    unsigned long long totalSpace = vfs.f_frsize * vfs.f_blocks;
    unsigned long long availableSpace = vfs.f_frsize * vfs.f_bavail;
    info.totalSpaceGB = static_cast<double>(totalSpace) / (1024 * 1024 * 1024);
    info.availableSpaceGB = static_cast<double>(availableSpace) / (1024 * 1024 * 1024);

    return info;
}

// Function to calculate disk space information, show the result, and write to a file
void calculateAndWriteDiskSpace(const std::string &diskPath, int numIterations)
{
    std::string filePath = "disk_space_info.txt"; // Fixed file path

    std::ofstream outFile(filePath, std::ios::app); // Open in append mode
    if (!outFile)
    {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (int i = 0; i < numIterations; ++i)
    {
        DiskSpaceInfo info = gatherDiskSpaceAvailability(diskPath);

        // Display the result
        std::cout << "Disk Path: " << diskPath << std::endl;
        std::cout << "Total Disk Space: " << info.totalSpaceGB << " GB\n";
        std::cout << "Available Disk Space: " << info.availableSpaceGB << " GB\n";
        std::cout << "-----------------------" << std::endl; // Separator

        // Write to the file
        outFile << "Disk Path: " << diskPath << std::endl;
        outFile << "Total Disk Space: " << info.totalSpaceGB << " GB\n";
        outFile << "Available Disk Space: " << info.availableSpaceGB << " GB\n";
        outFile << "-----------------------" << std::endl; // Separator

        // Delay for 2 seconds before the next calculation
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    std::cout << "Disk space information written to " << filePath << std::endl;

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

    // Calculate disk space information, show the result, and write to a file
    calculateAndWriteDiskSpace(diskPath, numIterations);

    return 0;
}
