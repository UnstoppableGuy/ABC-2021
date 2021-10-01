#include "MutexParallel.h"

void MutexParallel::increment()
{
	while (true) {
		_mutex.lock();
		int current_index = _index;
		_index++;
		_mutex.unlock();
		if (current_index < NumTasks) {
			byteArray[current_index] ++;
			std::this_thread::sleep_for(std::chrono::nanoseconds(10));
		}
		else {
			return;
		}
	}
}

void MutexParallel::start()
{
	_index = 0;
	setArray();
}
