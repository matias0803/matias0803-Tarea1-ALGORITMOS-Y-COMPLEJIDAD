#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <future>
#include <thread>
#include <memory>

void mergeSort(std::vector<int>& arr);
void quickSort(std::vector<int>& arr);
void patienceSort(std::vector<int>& arr);
std::vector<int> sortArray(std::vector<int>& arr);

namespace fs = std::filesystem;

struct FileParams {
    std::string n, t, d, m;
};

FileParams parseFilename(const std::string& filename) {
    FileParams fp;
    std::string base = filename.substr(0, filename.find_last_of("."));
    std::stringstream ss(base);
    std::getline(ss, fp.n, '_');
    std::getline(ss, fp.t, '_');
    std::getline(ss, fp.d, '_');
    std::getline(ss, fp.m, '_');
    return fp;
}


long getMemoryUsageKB() {
    std::ifstream statusFile("/proc/self/status");
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.rfind("VmRSS:", 0) == 0) {
            std::stringstream ss(line);
            std::string key;
            long val;
            ss >> key >> val;
            return val; 
        }
    }
    return 0;
}

int main() {
    std::string inputPath = "data/array_input/";
    std::string measurementsPath = "data/measurements/sorting_metrics.csv";

    fs::create_directories("data/measurements");

    std::ofstream metricsFile(measurementsPath, std::ios::app);
    metricsFile << "n,t,d,m,algorithm,time_ms,memory_kb\n";
    metricsFile.flush();

    int totalFiles = 0;
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (entry.path().filename().string() != "a.txt") totalFiles++;
    }

    int currentFileCount = 0;

    for (const auto& entry : fs::directory_iterator(inputPath)) {
        std::string filename = entry.path().filename().string();
        if (filename == "a.txt") continue;

        currentFileCount++;
        FileParams params = parseFilename(filename);

        std::cout << "\n----------------------------------------\n";
        std::cout << "[" << currentFileCount << " / " << totalFiles << "] Analizando: " << filename << "\n";

        std::vector<int> originalArray;
        std::ifstream inputFile(entry.path());
        int num;
        while (inputFile >> num) {
            originalArray.push_back(num);
        }
        inputFile.close();

        std::vector<std::string> algoNames = { "mergesort", "quicksort", "patiencesort", "stdsort" };

        for (const auto& algo : algoNames) {
            std::cout << "  -> Ejecutando: " << algo << "... " << std::flush;
            
            double timeMs = 0.0;
            long memoryUsed = 0;
            bool timedOut = false;

            long memBefore = getMemoryUsageKB();

            auto p = std::make_shared<std::promise<double>>();
            auto future = p->get_future();

            
            std::thread t([algo, arrCopy = originalArray, p]() mutable {
                auto start = std::chrono::high_resolution_clock::now();

                if (algo == "mergesort") mergeSort(arrCopy);
                else if (algo == "quicksort") quickSort(arrCopy);
                else if (algo == "patiencesort") patienceSort(arrCopy);
                else if (algo == "stdsort") sortArray(arrCopy);

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> duration = end - start;
                
                try {
                    p->set_value(duration.count());
                } catch (...) {}
            });

            t.detach();

          
            if (future.wait_for(std::chrono::minutes(5)) == std::future_status::timeout) {
                std::cout << "[TIMEOUT >5min]\n";
                timedOut = true;
                timeMs = 300000.0; 
                memoryUsed = 0;
            } else {
                timeMs = future.get();
                long memAfter = getMemoryUsageKB();
                memoryUsed = (memAfter > memBefore) ? (memAfter - memBefore) : 0;

                std::cout << "[OK] (" << timeMs << " ms, Mem: " << memoryUsed << " KB)\n";
            }

            // Escritura limpia en el CSV
            metricsFile << params.n << "," << params.t << "," << params.d << "," << params.m << "," 
                        << algo << "," << timeMs << "," << memoryUsed << "\n";
            metricsFile.flush(); 
        }
    }

    metricsFile.close();
    std::cout << "\n========================================\n";
    std::cout << "Ejecucion completada" << std::endl;
    return 0;
}
