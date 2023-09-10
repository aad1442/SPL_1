#include <iostream>
#include <string>
#include <fstream>
#include <sys/statvfs.h>
#include <thread>
#include <chrono>
#include <vector>
#include <iomanip>

struct SystemInfo {
    std::string diskPath;
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

    info.diskPath = path; // Store the disk path

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

// Function to write data to a file with formatted columns
void writeDataToFile(const std::string &filePath, const SystemInfo &info, bool writeHeader)
{
    std::ofstream outFile(filePath, std::ios::app); // Open in append mode
    if (!outFile)
    {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    // Set the column width based on the maximum width of headers and data
    int columnWidth = 50;

    // Write column headers if specified
    if (writeHeader)
    {
        outFile << std::left << std::setw(columnWidth) << "Disk Path"
                << std::setw(columnWidth) << "Total Disk Space (GB)"
                << std::setw(columnWidth) << "Available Disk Space (GB)"
                << std::setw(columnWidth) << "File System Block Size"
                << std::setw(columnWidth) << "Fundamental File System Block Size"
                << std::setw(columnWidth) << "Total Number of Blocks on File System"
                << std::setw(columnWidth) << "Total Number of Free Blocks"
                << std::setw(columnWidth) << "NoFB Available to Non-privileged Process"
                << std::setw(columnWidth) << "Total Number of File Serial Numbers"
                << std::setw(columnWidth) << "Total Number of Free File Serial Numbers"
                << std::setw(columnWidth) << "NoFFileSerialNAvailabletoNonprivilegedProcess"
                << std::setw(columnWidth) << "File System ID"
                << std::setw(columnWidth) << "File System Flags"
                << std::setw(columnWidth) << "Maximum File Name Length" << std::endl;
    }

    // Write data under the corresponding columns
    outFile << std::left << std::setw(columnWidth) << info.diskPath
            << std::fixed << std::setprecision(2) << std::setw(columnWidth) << info.totalSpaceGB
            << std::fixed << std::setprecision(2) << std::setw(columnWidth) << info.availableSpaceGB
            << std::setw(columnWidth) << info.f_bsize
            << std::setw(columnWidth) << info.f_frsize
            << std::setw(columnWidth) << info.f_blocks
            << std::setw(columnWidth) << info.f_bfree
            << std::setw(columnWidth) << info.f_bavail
            << std::setw(columnWidth) << info.f_files
            << std::setw(columnWidth) << info.f_ffree
            << std::setw(columnWidth) << info.f_favail
            << std::setw(columnWidth) << info.f_fsid
            << std::setw(columnWidth) << info.f_flag
            << std::setw(columnWidth) << info.f_namemax << std::endl;

    outFile.close();
}

// Function to calculate disk space and system information, show the result, and write to a file
void calculateAndWriteDiskSpace(const std::string &diskPath, int numIterations)
{
    std::string filePath = "disk_system_info.txt"; // Fixed file path

    for (int i = 0; i < numIterations; ++i)
    {
        SystemInfo info = gatherDiskAndSystemInfo(diskPath);

        // Write data to the file with headers only in the first iteration
        writeDataToFile(filePath, info, (i < 0));

        // Display the result
        std::cout << "Disk Path: " << info.diskPath << std::endl;
        std::cout << "Total Disk Space: " << info.totalSpaceGB << " GB\n";
        std::cout << "Available Disk Space: " << info.availableSpaceGB << " GB\n";
        std::cout << "-----------------------" << std::endl; // Separator

        // Delay for 2 seconds before the next calculation
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

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
    int numIterations = 10;

    // Calculate disk space and system information, show the result, and write to a file
    calculateAndWriteDiskSpace(diskPath, numIterations);

    return 0;
}
