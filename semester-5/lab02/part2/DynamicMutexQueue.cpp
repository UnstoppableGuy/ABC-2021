#include "DynamicMutexQueue.h"

void DynamicMutexQueue::push(uint8_t val)
{
    _mutex.lock();
    _mutexQueue.push(val);
    _mutex.unlock();
}

bool DynamicMutexQueue::pop(uint8_t& val)
{
    _mutex.lock();
    if (_mutexQueue.empty()) {
        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        return false;
    }
    val = _mutexQueue.front();
    _mutexQueue.pop();
    _mutex.unlock();
    return true;
}
