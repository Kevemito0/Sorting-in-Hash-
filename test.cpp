#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <unordered_map>
#include "hash.h"
#include <chrono>


int main() {
    std::string filename = "access_log";
    std::ifstream input(filename);

    if (!input.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    hash arr;
    
    const int maxEntries = 800000;
    LogEntry *arr_sort = new LogEntry[maxEntries];
    std::unordered_map<std::string ,LogEntry> unorderedMap;
    std::string line;
    int index = 0;
    std::chrono::time_point<std::chrono::system_clock> start,start1,end1, end;
    start = std::chrono::system_clock::now();
    while (getline(input, line) && index < maxEntries) {
        std::stringstream ss(line);
        LogEntry entry;
        std::string file_name;

        std::getline(ss, file_name, '"');
        std::getline(ss, file_name, ' ');
        std::getline(ss, file_name, ' ');
        entry.filename = file_name;
        arr.insert(entry);
        arr_sort[index].filename = file_name;
        arr_sort[index].visited = 1; // Initialize visited count to 1
        
        index++;
    }
    
    // Sort the array alphabetically
    std::sort(arr_sort, arr_sort + index, compareLogEntry);

    // Calculate the total visited count for each unique filename
    int totalVisited = 0;
    int uniqueCount = 0;
    FilenameCount *filenameCounts = new FilenameCount[index];

    for (int i = 0; i < index; ++i) {
        totalVisited += arr_sort[i].visited;
        if (i + 1 < index && arr_sort[i].filename != arr_sort[i + 1].filename) {
            // Store filename and total visited count in a new structure
            filenameCounts[uniqueCount].filename = arr_sort[i].filename;
            filenameCounts[uniqueCount].count = totalVisited;
            uniqueCount++;
            totalVisited = 0; // Reset totalVisited for the next filename
        }
    }

    // Sort the filenameCounts array alphabetically
    std::sort(filenameCounts, filenameCounts + uniqueCount, compareFilenameCount);

    // Print the stored filenames and their corresponding visit counts
    for (int i =uniqueCount -1; i > uniqueCount - 11; i--) {
        std::cout << "Filename: " << filenameCounts[i].filename << ", Visited: " << filenameCounts[i].count << std::endl;
    }
    end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_first = end - start;
    std::cout << "elapsed time without unordered map: " << elapsed_seconds_first.count() << "s\n";

    start1 = std::chrono::system_clock::now();
    while (getline(input, line) && index < maxEntries) {
        std::stringstream ss(line);
        LogEntry entry;
        std::string file_name;

        std::getline(ss, file_name, '"');
        std::getline(ss, file_name, ' ');
        std::getline(ss, file_name, ' ');
        entry.filename = file_name;
        unorderedMap[file_name] = entry;
        arr_sort[index].filename = file_name;
        arr_sort[index].visited = 1;
        
        index++;
    }
    // Sort the array alphabetically
    std::sort(arr_sort, arr_sort + index, compareLogEntry);

    // Calculate the total visited count for each unique filename
    int totalVisited1 = 0;
    int uniqueCount1 = 0;
    FilenameCount *filenameCountss = new FilenameCount[index];

    for (int i = 0; i < index; ++i) {
        totalVisited1 += arr_sort[i].visited;
        if (i + 1 < index && arr_sort[i].filename != arr_sort[i + 1].filename) {
            // Store filename and total visited count in a new structure
            filenameCountss[uniqueCount1].filename = arr_sort[i].filename;
            filenameCountss[uniqueCount1].count = totalVisited1;
            uniqueCount1++;
            totalVisited1 = 0; // Reset totalVisited for the next filename
        }
    }

    // Sort the filenameCounts array alphabetically
    std::sort(filenameCountss, filenameCountss + uniqueCount1, compareFilenameCount);

    // Print the stored filenames and their corresponding visit counts
    for (int i =uniqueCount -1; i > uniqueCount - 11; i--) {
        std::cout << "Filename: " << filenameCountss[i].filename << ", Visited: " << filenameCounts[i].count << std::endl;
    }
    end1 = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds_second = end1 - start1;
    std::cout << "elapsed time with unordered map: " << elapsed_seconds_second.count() << "s\n";

    delete[] arr_sort;
    return 0;
}
