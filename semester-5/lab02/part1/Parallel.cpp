#include "Parallel.h"

Parallel::Parallel()
{};

void Parallel::print()
{
    for (int i = 0; i < NumTasks; i++)
        std::cout << static_cast<int>(byteArray[i]) << " ";
}

void Parallel::setArray()
{
    byteArray = new uint8_t[NumTasks];
    for (int i = 0; i < NumTasks; i++)
        byteArray[i] = 0;
}

void Parallel::assertArray()
{
    for (int i = 0; i < NumTasks; i++)
        assert(byteArray[i] == 1);
}

Parallel::~Parallel()
{
    //std::cout << "Distuctor cold\n";
    delete[] byteArray;
}
