#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
using namespace std;

void generateFile(const string& directory, const string& filename, int Commands, int read0, int write0, int read1, int write1, int strPercent) 
{
    // створюємо папку, якщо вона не існує
    filesystem::create_directory(directory);

    ofstream file(directory + "/" + filename);
    srand(static_cast<unsigned int>(time(nullptr)));

    // псевдовипадково заповнюємо файл
    for (int i = 0; i < Commands; ++i) 
    {
        int chance = rand() % 100 + 1;
        if (chance <= read0) {
            file << "read 0" << endl;
        } else if (chance <= read0 + write0) {
            file << "write 0 1" << endl;
        } else if (chance <= read0 + write0 + read1) {
            file << "read 1" << endl;
        } else if (chance <= read0 + write0 + read1 + write1) {
            file << "write 1 1" << endl;
        } else {
            file << "string" << endl;
        }
    }
    file.close();
}
