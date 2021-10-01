#pragma once
#include "Parallel.h"
#include <chrono>
#include <thread>

class AtomicParallel : public Parallel {
private:

    std::atomic<int> _index;

    void start() override;

    void increment() override;
};
