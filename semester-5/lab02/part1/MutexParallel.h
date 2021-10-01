#pragma once
#include "Parallel.h"
#include <mutex>
class MutexParallel : public Parallel {
private:

    int _index = 0;

    std::mutex _mutex;

    void start() override;

    void increment() override;
 
};


