#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <iostream>
#include <mutex>
#include <string>
using namespace std;

class DataStructure {
private:
    int field0;
    int field1;
    mutex mtx0;
    mutex mtx1;

public:
    DataStructure() : field0(0), field1(0) {}

    int read(int field) 
    {
        if (field == 0) {
            lock_guard<mutex> lock(mtx0);
            return field0;
        }
        else if (field == 1) {
            lock_guard<mutex> lock(mtx1);
            return field1;
        }
        throw invalid_argument("Invalid field number");
    }

    void write(int field, int value) 
    {
        if (field == 0) {
            lock_guard<mutex> lock(mtx0);
            field0 = value;
        }
        else if (field == 1) {
            lock_guard<mutex> lock(mtx1);
            field1 = value;
        }
        else {
            throw invalid_argument("Invalid field number");
        }
    }

    // перевантаження оператора string
    operator string() 
    {
        lock_guard<mutex> lock0(mtx0);
        lock_guard<mutex> lock1(mtx1);
        return "Field 0: " + to_string(field0) + ", Field 1: " + to_string(field1);
    }
};

#endif // DATA_STRUCTURE_H

