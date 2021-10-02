#pragma once

#include <stdio.h>
#include "Queue.h"
#include <mutex>

class StaticMutexQueue : public Queue {
private:
    uint8_t* q;
    std::mutex _mutex;
    int _size;
    std::condition_variable _conditionPush, _conditionPop;
    std::atomic<int> _popIndex, _pushIndex, _elementsCount;

public:
    StaticMutexQueue(int val);

    void push(uint8_t val) override;

    bool pop(uint8_t& val) override; 

    ~StaticMutexQueue();
};

