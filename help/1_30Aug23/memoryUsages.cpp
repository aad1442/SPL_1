#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <chrono>

// Structure to store memory usage information
struct MemoryInfo {
    long totalMemory;
    long freeMemory;
    long buffers;
    long cached;
};

// Function to parse and store memory usage information
MemoryInfo parseMemoryInfo(const std::string& meminfoFilePath) {
    MemoryInfo info = {0, 0, 0, 0};
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
            }
        }
    }

    return info;
}

// Function to display memory usage information
void displayMemoryInfo(const MemoryInfo& info, int iteration) {
    std::cout << "Memory Usage (Iteration " << iteration << "):\n";
    std::cout << "Total Memory: " << info.totalMemory << " MB\n";
    std::cout << "Free Memory: " << info.freeMemory << " MB\n";
    std::cout << "Buffers: " << info.buffers << " MB\n";
    std::cout << "Cached: " << info.cached << " MB\n";
}

// Function to write memory usage information to a file
void writeMemoryInfoToFile(const MemoryInfo& info, int iteration, std::ofstream& outFile) {
    outFile << "Memory Usage (Iteration " << iteration << "):\n";
    outFile << "Total Memory: " << info.totalMemory << " MB\n";
    outFile << "Free Memory: " << info.freeMemory << " MB\n";
    outFile << "Buffers: " << info.buffers << " MB\n";
    outFile << "Cached: " << info.cached << " MB\n";
    outFile << "\n"; // Add a newline to separate entries
}

// Function to retrieve memory usage values five times with a 1-second interval and write to a file
void retrieveAndWriteMemoryUsage(const std::string& meminfoFilePath) {
    std::ofstream outFile("memory_usage.txt",std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open the output file for writing\n";
        return;
    }

    for (int i = 1; i <= 5; ++i) {
        MemoryInfo memoryInfo = parseMemoryInfo(meminfoFilePath);
        displayMemoryInfo(memoryInfo, i);
        writeMemoryInfoToFile(memoryInfo, i, outFile);

        if (i < 5) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    outFile.close();
}

int main() {
    std::string meminfoFilePath = "/proc/meminfo"; // Path to the memory info file
    retrieveAndWriteMemoryUsage(meminfoFilePath);

    return 0;
}
