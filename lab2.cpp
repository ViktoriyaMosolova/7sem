// #include <iostream>
// #include <vector>
// #include <thread>
// #include <mutex>
// #include <random>
// #include <chrono>
// using namespace std;

// std::mutex mut;

// void threadCallback(const vector<string>& data, int start, int end, int& totalNines) {
//     for (int i = start; i < end; ++i) {
//         int countNines = count(data[i].begin(), data[i].end(), '9');
        
//         mut.lock();
//         totalNines += countNines;
//         mut.unlock();
//     }
// }

// string generateRandomNumbers(int length) {
//     static random_device rd;
//     static mt19937 gen(rd());
//     static uniform_int_distribution<> dis(0, 9);
    
//     string result;
//     result.reserve(length);
//     for (int i = 0; i < length; ++i) {
//         result += to_string(dis(gen));
//     }
//     return result;
// }

// void processWithThreads(const vector<string>& data, int n, int& totalNines) {
//     int chunkSize = data.size() / n;
//     vector<thread> threads;

//     for (int i = 0; i < n; ++i) {
//         int start = i * chunkSize;
//         int end = (i == n - 1) ? data.size() : (i + 1) * chunkSize;
        
//         threads.push_back(std::thread(threadCallback, std::ref(data), start, end, std::ref(totalNines)));
//     }

//     for (auto& t : threads) {
//         t.join();
//     }
// }

// int main() {
//     const int numRows = 300;
//     const int rowLength = 200000;
//     const vector<int> nValues = {1, 2, 4, 10};

//     // Генерация случайных данных
//     vector<string> data;
//     data.reserve(numRows);
//     for (int i = 0; i < numRows; ++i) {
//         data.push_back(generateRandomNumbers(rowLength));
//     }

//     for (int n : nValues) {
//         int totalNines = 0;
        
//         auto start = chrono::system_clock::now();
//         processWithThreads(data, n, totalNines);
//         auto end = chrono::system_clock::now();
        
//         auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
//         cout << "Threads: " << n << ", Elapsed time: " << elapsed << " ms, Total Nines: " << totalNines << endl;
//     }

//     return 0;
// }

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <omp.h>
#include <algorithm>
using namespace std;

string generateRandomNumbers(int length) {
    std::srand(std::time(nullptr));

    string result;
    result.reserve(length);
    for (int i = 0; i < length; ++i) {
        result += to_string(std::rand() % 10);
    }
    return result;
}

int main() {
    const int numRows = 300;
    const int rowLength = 200000;
    const vector<int> nValues = {1, 2, 4, 10, 16};

    // Генерация случайных данных
    vector<string> data;
    data.reserve(numRows);
    for (int i = 0; i < numRows; ++i) {
        data.push_back(generateRandomNumbers(rowLength));
    }

    for (int n : nValues) {
        int totalNines = 0;
        auto start = chrono::system_clock::now();

        // Параллельный блок OpenMP
        #pragma omp parallel for num_threads(n) reduction(+:totalNines)
        for (int i = 0; i < numRows; ++i) {
            int countNines = count(data[i].begin(), data[i].end(), '9');
            totalNines += countNines;
        }

        auto end = chrono::system_clock::now();
        auto elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        cout << "Threads: " << n << ", Elapsed time: " << elapsed << " ms, Total Nines: " << totalNines << endl;
    }

    return 0;
}

