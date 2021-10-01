#include "MutexParallel.h"
#include "AtomicParallel.h"
#include <vector>

void runTask(Parallel& parallel, int numThreads) {
	parallel.start();

	std::vector<std::thread> threads;

	for (int i = 0; i < numThreads; i++)
		threads.push_back(std::thread([&]() { parallel.increment(); }));

	for (int i = 0; i < numThreads; i++)
		threads[i].join();

	parallel.assertArray();
}

void getTime(Parallel& parallel, int numThreads) {
	auto startTime = std::chrono::steady_clock::now();

	runTask(parallel, numThreads);

	auto endTime = std::chrono::steady_clock::now();

	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

	std::cout << "Runnung time of " << numThreads << " threads: "
		<< (double)time.count() / 1000 << std::endl;
	std::cout << "==================================\n";
}

int main() {
	MutexParallel mutexParallel;
	std::vector<int> threadNums = { 4, 8, 16, 32 };
	std::cout << "\t\tMutex\n==================================\n";
	for (auto num : threadNums)
		getTime(mutexParallel, num);

	std::cout << "\n\t\tAtomic\n==================================\n";
	AtomicParallel atomicParallel;
	for (auto num : threadNums)
		getTime(atomicParallel, num);

	return 0;
}