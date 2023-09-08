#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

void monitor_and_record_cpu_usage() {
    FILE* top_output = popen("top -n 1 -b | grep '%Cpu(s)' | awk '{print $2}'", "r");
    if (!top_output) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), top_output) != nullptr) {
        // Extracted CPU usage as a string from the command output
        double cpu_usage = strtod(buffer, nullptr);
        
        // Create or open the file for writing
        std::ofstream file("cpu_usage_log.txt", std::ios::app); // Append mode

        if (file.is_open()) {
            // Write CPU usage to the file
            file << "CPU Usage: " << cpu_usage << "%" << std::endl;
            file.close();
        } else {
            std::cerr << "Failed to open the file for writing" << std::endl;
        }

        std::cout << "Current CPU Usage: " << cpu_usage << "%" << std::endl;
    } else {
        std::cerr << "Failed to read CPU usage" << std::endl;
    }

    pclose(top_output);
}

int main() {
    monitor_and_record_cpu_usage();
    return 0;
}
