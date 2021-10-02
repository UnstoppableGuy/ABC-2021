#include "Queue.h"
#include <mutex>
#include <queue>

class DynamicMutexQueue : public Queue {
private:
	std::queue<uint8_t> _mutexQueue;
	std::mutex _mutex;

public:
	void push(uint8_t val) override;

	bool pop(uint8_t& val) override;
};

