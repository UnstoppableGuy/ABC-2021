#include "AtomicParallel.h"


void AtomicParallel::start()
{
	_index = 0;
	setArray();
}

void AtomicParallel::increment()
{
	while (true) {
		auto prev = _index.fetch_add(1);
		if (prev >= NumTasks)
			return;
		byteArray[prev]++;
		std::this_thread::sleep_for(std::chrono::nanoseconds(10));
	}
}
