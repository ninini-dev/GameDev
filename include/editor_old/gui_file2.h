#pragma once
#include <vector>
#include <fstream>
using namespace std;

template<typename T>
void SaveVT(vector<T>& list, const char* file) {
    std::ofstream outFile(file, std::ios::binary);

    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<char*>(list.data()),list.size()*sizeof(T));
    }
    outFile.close();
}
template <typename T>
void LoadVT(vector<T>& list, const char* file) {

    std::ifstream inputFile(file, std::ios::binary | std::ios::in|std::ios::ate);

    if (inputFile.is_open()) {
        size_t size=inputFile.tellg();
        inputFile.seekg(std::ios::beg);
        size -= inputFile.tellg();
        list.resize(size / sizeof(T));
        inputFile.read(reinterpret_cast<char*>(list.data()), size);

    }
    inputFile.close();
}

template<typename T>
void SaveVVT(vector<vector<T>>& list, const char* file ) {
    std::ofstream outFile(file, std::ios::binary);

    if (outFile.is_open()) {
        for (size_t i = 0; i < list.size(); i++)
        {
            size_t size = list[i].size() * sizeof(T);
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size_t));
            outFile.write(reinterpret_cast<char*>(list[i].data()), size);
        }

    }
    outFile.close();
}
template <typename T>
void LoadVVT(vector<vector<T>>& list, const char* file) {

    std::ifstream inputFile(file, std::ios::binary | std::ios::in);

    if (inputFile.is_open()) {
        size_t i = 0;
        list.clear();

        size_t test;
        while (!inputFile.eof())
        {
            inputFile.read(reinterpret_cast<char*>(&test), sizeof(size_t));

            list.push_back({});
            list[i].resize(test / sizeof(T));
            inputFile.read(reinterpret_cast<char*>(list[i].data()), test);
            i++;
        }
        list.pop_back();
    }
    inputFile.close();
}