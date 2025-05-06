#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <sstream>
#include <iomanip>

struct LogEntry {
    std::string timestamp;
    std::string service;
    std::string message;
    std::string priority;
    int boot_id;
};

class LogAnalyzer {
    std::vector<LogEntry> entries;
    std::map<int, std::map<std::string, std::vector<LogEntry>>> tree;

public:
    void loadLogs() {
        std::string cmd = "journalctl --no-pager -o json --all";
        std::string jsonData = exec(cmd);
        
        // Parse JSON (simplified for example)
        // In real implementation, use proper JSON parsing
        parseJournalJSON(jsonData);
    }

    void buildTree() {
        for (const auto& entry : entries) {
            tree[entry.boot_id][entry.service].push_back(entry);
        }
    }

    void displayTree() {
        for (const auto& [boot, services] : tree) {
            std::cout << "┌── Boot " << boot << "\n";
            
            for (const auto& [service, logs] : services) {
                std::cout << "│   ├── " << service << "\n";
                
                for (const auto& log : logs) {
                    std::cout << "│   │   ├── [" << log.priority << "] "
                              << log.timestamp << ": " 
                              << log.message.substr(0, 50) 
                              << (log.message.length() > 50 ? "..." : "")
                              << "\n";
                }
            }
        }
    }

private:
    std::string exec(const std::string& cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (fgets(buffer.data(), buffer.size(), pipe.get())) {
            result += buffer.data();
        }
        return result;
    }

    void parseJournalJSON(const std::string& json) {
        // Simplified parsing - in reality use nlohmann/json or similar
        // This just demonstrates the structure
        LogEntry sample;
        sample.timestamp = "2023-07-20 10:00:00";
        sample.service = "sshd.service";
        sample.message = "Accepted password for user from 192.168.1.1";
        sample.priority = "WARNING";
        sample.boot_id = 1;
        entries.push_back(sample);
        
        // Add more sample data
        LogEntry sample2;
        sample2.timestamp = "2023-07-20 10:01:00";
        sample2.service = "crond.service";
        sample2.message = "Job completed successfully";
        sample2.priority = "INFO";
        sample2.boot_id = 1;
        entries.push_back(sample2);
    }
};

int main() {
    LogAnalyzer analyzer;
    analyzer.loadLogs();
    analyzer.buildTree();
    analyzer.displayTree();
    return 0;
}
