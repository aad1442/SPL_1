#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <ctime>

// Structure to store memory usage information
struct MemoryInfo {
    long totalMemory;
    long freeMemory;
    long buffers;
    long cached;
    long active;
    long inactive;
    std::time_t timestamp; // Time when memory info is recorded
};

// Function to parse and store memory usage information
MemoryInfo parseMemoryInfo(const std::string& meminfoFilePath) {
    MemoryInfo info = {0, 0, 0, 0, 0, 0, 0};
    std::ifstream meminfoFile(meminfoFilePath);

    if (!meminfoFile.is_open()) {
        std::cerr << "Failed to open " << meminfoFilePath << "\n";
        return info;
    }

    std::string line;
    while (std::getline(meminfoFile, line)) {
        std::istringstream iss(line);
        std::string key;
        long value;

        if (iss >> key >> value) {
            if (key == "MemTotal:") {
                info.totalMemory = value / 1024; // Convert from KB to MB
            } else if (key == "MemFree:") {
                info.freeMemory = value / 1024; // Convert from KB to MB
            } else if (key == "Buffers:") {
                info.buffers = value / 1024; // Convert from KB to MB
            } else if (key == "Cached:") {
                info.cached = value / 1024; // Convert from KB to MB
            } else if (key == "Active:") {
                info.active = value / 1024; // Convert from KB to MB
            } else if (key == "Inactive:") {
                info.inactive = value / 1024; // Convert from KB to MB
            }
        }
    }

    // Record the current time
    info.timestamp = std::time(nullptr);

    return info;
}

// Function to write memory usage information to a file with adjusted setw values
void writeMemoryInfoToFile(const std::vector<MemoryInfo>& infoVec) {
    std::ofstream outFile("memory_usage.txt");
    if (!outFile.is_open()) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    outFile << std::left << std::setw(40) << "Timestamp"
            << std::setw(25) << "Total Memory (MB)" << std::setw(25) << "Free Memory (MB)"
            << std::setw(25) << "Buffers (MB)" << std::setw(25) << "Cached (MB)"
            << std::setw(25) << "Active (MB)" << std::setw(25) << "Inactive (MB)" << "\n";

    for (int i = 0; i < infoVec.size(); ++i) {
        std::string timestampStr = std::asctime(std::localtime(&infoVec[i].timestamp));
        timestampStr.pop_back(); // Remove the trailing newline

        std::cout << "Memory Usage (Iteration " << (i + 1) << "):\n";
        std::cout << "Timestamp: " << timestampStr << "\n";
        std::cout << "Total Memory: " << infoVec[i].totalMemory << " MB\n";
        std::cout << "Free Memory: " << infoVec[i].freeMemory << " MB\n";
        std::cout << "Buffers: " << infoVec[i].buffers << " MB\n";
        std::cout << "Cached: " << infoVec[i].cached << " MB\n";
        std::cout << "Active: " << infoVec[i].active << " MB\n";
        std::cout << "Inactive: " << infoVec[i].inactive << " MB\n";
        std::cout << "-----------------------\n";

        outFile << std::left << std::setw(40) << timestampStr
                << std::setw(25) << infoVec[i].totalMemory << std::setw(25) << infoVec[i].freeMemory
                << std::setw(25) << infoVec[i].buffers << std::setw(25) << infoVec[i].cached
                << std::setw(25) << infoVec[i].active << std::setw(25) << infoVec[i].inactive << "\n";
    }

    outFile.close();
}

// Function to retrieve memory usage values five times with a 1-second interval and store in a vector
void retrieveMemoryUsage(const std::string& meminfoFilePath, std::vector<MemoryInfo>& infoVec) {
    for (int i = 0; i < 5; ++i) {
        MemoryInfo memoryInfo = parseMemoryInfo(meminfoFilePath);
        infoVec.push_back(memoryInfo);

        if (i < 4) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    std::string meminfoFilePath = "/proc/meminfo"; // Path to the memory info file
    std::vector<MemoryInfo> memoryInfoVec;

    // Retrieve memory usage information and store in the vector
    retrieveMemoryUsage(meminfoFilePath, memoryInfoVec);

    // Write memory usage information from the vector to a file and print it
    writeMemoryInfoToFile(memoryInfoVec);

    return 0;
}
