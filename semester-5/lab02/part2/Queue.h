#pragma once
#include <cstdint>

class Queue
{
public:
	virtual void push(uint8_t val) = 0;
	
	virtual bool pop(uint8_t& val) = 0;
};

