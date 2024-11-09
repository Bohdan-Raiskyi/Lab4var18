#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <chrono>
#include <filesystem>
#include "DataStructure.h"
using namespace std;

void generateFile(const string& directory, const string& filename, int Commands, int read0, int write0, int read1, int write1, int strPercent);

// функція обробки частини(всього) файлу в 1 потоці
void executeFilePart(const string& filepath, DataStructure& dataStruct, streampos start, streampos end, chrono::milliseconds& time) {
    ifstream file(filepath);
    file.seekg(start);
    string command;
    auto startTime = chrono::high_resolution_clock::now();

    while (file.tellg() < end && file >> command) {
        if (command == "read") {
            int field;
            file >> field;
            dataStruct.read(field);
        }
        else if (command == "write") {
            int field, value;
            file >> field >> value;
            dataStruct.write(field, value);
        }
        else if (command == "string") {
            string str = dataStruct;
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    //замір частини часу
    time = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);
}

// розділення на частини
void processFileInParts(const string& filepath, DataStructure& dataStruct, int K) {
    ifstream file(filepath);
    file.seekg(0, ios::end);
    streampos fileSize = file.tellg();
    file.close();

    vector<thread> threads;
    vector<chrono::milliseconds> times(K);  // сума замірів

    streampos partSize = fileSize / K; // розділення на потоки
    for (int i = 0; i < K; ++i) {
        streampos start = i * partSize;
        streampos end = (i == K - 1) ? fileSize : start + partSize;

        threads.emplace_back(executeFilePart, filepath, ref(dataStruct), start, end, ref(times[i]));
    }

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // загальний час
    chrono::milliseconds totalTime(0);
    for (const auto& t : times) {
        totalTime += t;
    }

    cout << totalTime.count() << " ms\n";
}

// обробка всіх файлів в папці
void processDirectory(const string& directory, DataStructure& dataStruct, int K) {
    for (const auto& entry : filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            cout << "Обробка файлу: " << entry.path().filename() << " з використанням " << K << " потоків ";
            processFileInParts(entry.path().string(), dataStruct, K);
        }
    }
}

int main() 
{
    setlocale(LC_CTYPE, "ukr");

    DataStructure dataStruct;
    string directory = "generated_files";
    filesystem::create_directory(directory);

    generateFile(directory, "file1.txt", 10000, 20, 10, 25, 10, 35);
    generateFile(directory, "file2.txt", 10000, 20, 10, 25, 10, 35);
    generateFile(directory, "file3.txt", 10000, 20, 10, 25, 10, 35);

    cout << "Обробка в одному потоці з частотами як в умові" << endl;
    processDirectory(directory, dataStruct, 1);
   
    cout << endl << "Обробка в двох потоках з частотами як в умові" << endl;
    processDirectory(directory, dataStruct, 2);

    cout << endl << "Обробка в трьох потоках з частотами як в умові" << endl;
    processDirectory(directory, dataStruct, 3);

    generateFile(directory, "file1.txt", 10000, 20, 20, 20, 20, 20);
    generateFile(directory, "file2.txt", 10000, 20, 20, 20, 20, 20);
    generateFile(directory, "file3.txt", 10000, 20, 20, 20, 20, 20);

    cout << endl << "Обробка в одному потоці з рівними частотами" << endl;
    processDirectory(directory, dataStruct, 1);

    cout << endl << "Обробка в двох потоках з рівними частотами" << endl;
    processDirectory(directory, dataStruct, 2);

    cout << endl << "Обробка в трьох потоках з рівними частотами" << endl;
    processDirectory(directory, dataStruct, 3);

    generateFile(directory, "file1.txt", 10000, 5, 60, 10, 3, 22);
    generateFile(directory, "file2.txt", 10000, 5, 60, 10, 3, 22);
    generateFile(directory, "file3.txt", 10000, 5, 60, 10, 3, 22);

    cout << endl << "Обробка в одному потоці з власними частотами" << endl;
    processDirectory(directory, dataStruct, 1);

    cout << endl << "Обробка в двох потоках з власними частотами" << endl;
    processDirectory(directory, dataStruct, 2);

    cout << endl << "Обробка в трьох потоках з власними частотами" << endl;
    processDirectory(directory, dataStruct, 3);

    return 0;
}
