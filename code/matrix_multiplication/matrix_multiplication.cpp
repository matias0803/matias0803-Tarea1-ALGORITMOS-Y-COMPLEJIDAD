#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <filesystem>
#include <unistd.h>


typedef long long lld;

// Firmas de tus algoritmos
std::vector<std::vector<int>> naiveMultiply(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B);
lld** Strassen(lld** a, lld** b, int n, int l, int m);

namespace fs = std::filesystem;

struct FileParams {
    std::string n, t, d, m;
};


double getMemoryUsageKB() {
    std::ifstream statStream("/proc/self/status");
    std::string line;
    while (std::getline(statStream, line)) {
        if (line.compare(0, 6, "VmRSS:") == 0) {
            std::stringstream ss(line);
            std::string ignore;
            double vmRSS;
            ss >> ignore >> vmRSS; 
            return vmRSS;
        }
    }
    return 0.0; 
}

FileParams parseMatrixFilename(const std::string& filename) {
    FileParams fp;
    std::string base = filename.substr(0, filename.find_last_of('_'));
    std::stringstream ss(base);
    std::getline(ss, fp.n, '_');
    std::getline(ss, fp.t, '_');
    std::getline(ss, fp.d, '_');
    std::getline(ss, fp.m, '_');
    return fp;
}

std::vector<std::vector<int>> readMatrix(const std::string& path, int n) {
    std::vector<std::vector<int>> mat(n, std::vector<int>(n));
    std::ifstream file(path);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> mat[i][j];
        }
    }
    file.close();
    return mat;
}


void writeOutputVector(const std::string& path, const std::vector<std::vector<int>>& mat) {
    std::ofstream file(path);
    int n = mat.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << mat[i][j] << (j == n - 1 ? "" : " ");
        }
        file << "\n";
    }
    file.close();
}


void writeOutputPtr(const std::string& path, lld** mat, int n) {
    std::ofstream file(path);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << mat[i][j] << (j == n - 1 ? "" : " ");
        }
        file << "\n";
    }
    file.close();
}


lld** vectorToPtr(const std::vector<std::vector<int>>& mat) {
    int n = mat.size();
    lld** p = new lld*[n];
    for (int i = 0; i < n; i++) {
        p[i] = new lld[n];
        for (int j = 0; j < n; j++) {
            p[i][j] = mat[i][j];
        }
    }
    return p;
}

void freePtr(lld** p, int n) {
    for (int i = 0; i < n; i++) {
        delete[] p[i];
    }
    delete[] p;
}


int main() {
    std::string inputPath = "data/matrix_input/";
    std::string measurementsPath = "data/measurements/matrix_metrics.csv";
    std::string outDir = "./data/matrix_output/";

    fs::create_directories("data/measurements");
    fs::create_directories(outDir); 

    std::ofstream metricsFile(measurementsPath, std::ios::app);
    metricsFile << "n,t,d,m,algorithm,time_ms,memory_kb\n";
    metricsFile.flush();

    int totalPairs = 0;
    for (const auto& entry : fs::directory_iterator(inputPath)) {
        if (entry.path().filename().string().find("_1.txt") != std::string::npos) {
            totalPairs++;
        }
    }

    int currentPairCount = 0;

    for (const auto& entry : fs::directory_iterator(inputPath)) {
        std::string filename1 = entry.path().filename().string();
        
        if (filename1.find("_1.txt") == std::string::npos) continue;

        currentPairCount++;
        FileParams params = parseMatrixFilename(filename1);
        int n_size = std::stoi(params.n);
        std::string baseName = filename1.substr(0, filename1.find_last_of('_'));
        std::string filename2 = baseName + "_2.txt";

        std::cout << "\n----------------------------------------\n";
        std::cout << "[" << currentPairCount << " / " << totalPairs << "] Multiplicando: " << baseName << "\n";

        
        auto matA = readMatrix(inputPath + filename1, n_size);
        auto matB = readMatrix(inputPath + filename2, n_size);

        
        std::cout << "  -> Ejecutando: naive... " << std::flush;
        double memBeforeNaive = getMemoryUsageKB();
        auto startNaive = std::chrono::high_resolution_clock::now();
        
        auto resultNaive = naiveMultiply(matA, matB);
        
        auto endNaive = std::chrono::high_resolution_clock::now();
        double memAfterNaive = getMemoryUsageKB();
        double timeNaive = std::chrono::duration<double, std::milli>(endNaive - startNaive).count();
        double memNaiveUsed = (memAfterNaive > memBeforeNaive) ? (memAfterNaive - memBeforeNaive) : 0.0;
        std::cout << "[OK] (" << timeNaive << " ms, Mem: " << memNaiveUsed << " KB)\n";
        
        
        std::string naiveOutName = outDir + baseName + "_naive_out.txt";
        writeOutputVector(naiveOutName, resultNaive);

        metricsFile << params.n << "," << params.t << "," << params.d << "," << params.m << ",naive," << timeNaive << "," << memNaiveUsed << "\n";
        metricsFile.flush();

       
        std::cout << "  -> Ejecutando: strassen... " << std::flush;
        lld** ptrA = vectorToPtr(matA);
        lld** ptrB = vectorToPtr(matB);

        double memBeforeStrassen = getMemoryUsageKB();
        auto startStrassen = std::chrono::high_resolution_clock::now();
        
        lld** ptrC = Strassen(ptrA, ptrB, n_size, n_size, n_size);
        
        auto endStrassen = std::chrono::high_resolution_clock::now();
        double memAfterStrassen = getMemoryUsageKB();
        double timeStrassen = std::chrono::duration<double, std::milli>(endStrassen - startStrassen).count();
        double memStrassenUsed = (memAfterStrassen > memBeforeStrassen) ? (memAfterStrassen - memBeforeStrassen) : 0.0;
        std::cout << "[OK] (" << timeStrassen << " ms, Mem: " << memStrassenUsed << " KB)\n";

        
        std::string strassenOutName = outDir + baseName + "_strassen_out.txt";
        writeOutputPtr(strassenOutName, ptrC, n_size);

        metricsFile << params.n << "," << params.t << "," << params.d << "," << params.m << ",strassen," << timeStrassen << "," << memStrassenUsed << "\n";
        metricsFile.flush();

        // Limpieza de memoria
        freePtr(ptrA, n_size);
        freePtr(ptrB, n_size);
        freePtr(ptrC, n_size);
    }

    metricsFile.close();
    std::cout << "\n========================================\n";
    std::cout << "Ejecucion completada exitosamente." << std::endl;
    return 0;
}