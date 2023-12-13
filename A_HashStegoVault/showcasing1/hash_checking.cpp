#include <iostream>
#include <fstream>
#include <string>

bool compareFiles(const std::string& filePath1, const std::string& filePath2) {
    // Open the first file
    std::ifstream file1(filePath1);
    if (!file1.is_open()) {
        std::cerr << "Error opening file: " << filePath1 << std::endl;
        return false;
    }

    // Open the second file
    std::ifstream file2(filePath2);
    if (!file2.is_open()) {
        std::cerr << "Error opening file: " << filePath2 << std::endl;
        return false;
    }

    // Read the contents of the first file
    std::string content1((std::istreambuf_iterator<char>(file1)),
                         std::istreambuf_iterator<char>());

    // Read the contents of the second file
    std::string content2((std::istreambuf_iterator<char>(file2)),
                         std::istreambuf_iterator<char>());

    // Close the files
    file1.close();
    file2.close();

    // Check if the contents are the same
    return content1 == content2;
}

int main() {
    // File paths
    std::string filePath1 = "input.txt";
    std::string filePath2 = "input.txt";

    // Call the function to compare files
    if (compareFiles(filePath1, filePath2)) {
        std::cout << "Both files have the same content." << std::endl;
    } else {
        std::cout << "The contents of the files are different." << std::endl;
    }

    return 0;
}
