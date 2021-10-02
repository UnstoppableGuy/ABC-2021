#pragma once

#include <stdio.h>
#include "Queue.h"
#include <atomic>

class StaticAtomicQueue : public Queue {
private:
    std::atomic<uint8_t>* _atomicQueue;
    int _size;
    std::atomic<int> _pushIndex, _popIndex;

public:
    StaticAtomicQueue(int val);

    void push(uint8_t val) override;

    bool pop(uint8_t& val) override;

    ~StaticAtomicQueue();
};

