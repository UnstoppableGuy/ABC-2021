#pragma once

#include <cstdint>
#include <iostream>
#include <cassert>

class Parallel {

private:

public:
    const int NumTasks = 1024 * 1024;

    uint8_t* byteArray;
    
    Parallel(); 

    void print();

    void setArray();

    void assertArray();

    virtual void start() = 0;

    virtual void increment() = 0;

    ~Parallel();
};

