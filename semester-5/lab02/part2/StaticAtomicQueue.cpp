#include "StaticAtomicQueue.h"

StaticAtomicQueue::StaticAtomicQueue(int val)
{
    _pushIndex = 0;
    _popIndex = 0;
    _size = val;
    _atomicQueue = new std::atomic<uint8_t>[_size];
    for (int i = 0; i < _size; i++)
        _atomicQueue[i] = 0;
}

void StaticAtomicQueue::push(uint8_t val)
{
    while (true) {
        int currentPushIndex = _pushIndex.load();
        if (currentPushIndex == _popIndex.load() + _size) {
            continue;
        }

        uint8_t x = _atomicQueue[currentPushIndex % _size];
        if (x != 0) continue;

        if (_pushIndex.compare_exchange_strong(currentPushIndex,
            currentPushIndex + 1)) {
            if (_atomicQueue[currentPushIndex % _size].compare_exchange_strong(x, val))
                return;
        }
    }
}

bool StaticAtomicQueue::pop(uint8_t& val)
{
    int currentPopIndex = _popIndex.load();
    if (currentPopIndex == _pushIndex.load()) {
        return false;
    }

    uint8_t x = _atomicQueue[currentPopIndex % _size];
    if (x == 0) return false;
    if (_popIndex.compare_exchange_strong(currentPopIndex,
        currentPopIndex + 1)) {

        if (_atomicQueue[currentPopIndex % _size].compare_exchange_strong(x, 0)) {
            val = x;
            return true;
        }
    }
    return false;
}

StaticAtomicQueue::~StaticAtomicQueue()
{
    delete[] _atomicQueue;
}
