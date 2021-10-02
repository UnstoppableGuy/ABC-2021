#include "StaticMutexQueue.h"

StaticMutexQueue::StaticMutexQueue(int val)
{
    _size = val;
    q = new uint8_t[_size];
    for (int i = 0; i < _size; i++)
        q[i] = 0;
    _elementsCount = 0;
    _popIndex = 0;
    _pushIndex = 0;
}

void StaticMutexQueue::push(uint8_t val)
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (_elementsCount == _size)
        _conditionPush.wait(lock);
    q[_pushIndex % _size] = val;
    _pushIndex++;
    _elementsCount++;
    _conditionPop.notify_one();
}

bool StaticMutexQueue::pop(uint8_t& val)
{
    std::unique_lock<std::mutex> lock(_mutex);
    while (_elementsCount == 0)
        _conditionPop.wait(lock);
    val = q[_popIndex % _size];
    q[_popIndex % _size] = 0;
    _popIndex++;
    _elementsCount--;
    _conditionPush.notify_one();
    return true;
}

StaticMutexQueue::~StaticMutexQueue()
{
    delete[] q;
}
