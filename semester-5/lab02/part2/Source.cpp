#include "DynamicMutexQueue.h"
#include "StaticMutexQueue.h"
#include "StaticAtomicQueue.h"

#include <cassert>
#include <iostream>
#include <chrono>

std::vector<int> consumerNums({ 1, 2, 4 });
std::vector<int> produserNums({ 1, 2, 4 });
std::vector<int> sizes({/* 1, 4, */16 });
std::vector<std::vector<double>> times(3);

void runTask(Queue& q, int producerNum, int consumerNum, int taskNum) {
    std::atomic<int> sum(0);

    auto producer = [&]() {
        for (int i = 0; i < taskNum; i++) {
            q.push(1);
        }
    };

    auto consumer = [&]() {
        for (int i = 0; i < taskNum * producerNum / consumerNum; i++) {
            uint8_t poppedValue = 0;
            while (!q.pop(poppedValue));
            sum += poppedValue;
        }
    };

    std::vector<std::thread> threads;

    for (int i = 0; i < producerNum; i++)
        threads.push_back(std::thread(producer));

    for (int i = 0; i < consumerNum; i++)
        threads.push_back(std::thread(consumer));

    for (int i = 0; i < consumerNum + producerNum; i++)
        threads[i].join();

    std::cout << "consumer: " << consumerNum << " producer: " << producerNum /* << std::endl*/;
    if (sum != taskNum * producerNum)
    {
        std::cout << "Error in calculating";
        return;
    }
    //assert(sum == taskNum * producerNum);
}

void getTime(Queue& q, int i) {

    int taskNum = 4 * 1024 * 1024;
    for (auto consumer : consumerNums) {
        for (auto producer : produserNums) {
            auto start = std::chrono::high_resolution_clock::now();
            runTask(q, producer, consumer, taskNum);
            auto end = std::chrono::high_resolution_clock::now();
            auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            times[i].push_back((double)time.count() / 1000);
            std::cout << " time: " << (double)time.count() / 1000 << " s\n";
            std::cout << "========================================\n";
        }
    }
}


int main() {

    DynamicMutexQueue dynamicMutexQueue;
    std::cout << "\tDynamic Mutex Queue\n";
    getTime(dynamicMutexQueue,0);

    std::cout << "\tStatic Mutex Queue\n";
    for (auto size : sizes) {
        StaticMutexQueue staticMutexQueue(size);
        std::cout << "\t\tsize: " << size << std::endl;
        getTime(staticMutexQueue,1);
    }
    //std::cout << "\tStatic Atomic Queue\n";
    //for (auto size : sizes) {
    //    StaticAtomicQueue staticAtomicQueue(size);
    //    //std::cout << "size: " << size << std::endl;
    //    getTime(staticAtomicQueue,2);
    //}

    return 0;
}