
#ifndef RISCV_SIM_CPU_H
#define RISCV_SIM_CPU_H

#include "Memory.h"
#include "Decoder.h"
#include "RegisterFile.h"
#include "CsrFile.h"
#include "Executor.h"

class Cpu
{
public:
    Cpu(Memory &mem)
        : _mem(mem)
    {
    }

    void ProcessInstruction()
    {

        InstructionPtr instrDec = _decoder.Decode(_mem.Request(_ip));
        _rf.Read(instrDec);
        _csrf.Read(instrDec);
        _exe.Execute(instrDec, _ip);
        _mem.Request(instrDec);
        _rf.Write(instrDec);
        _csrf.Write(instrDec);
        _csrf.InstructionExecuted();
        _ip = instrDec->_nextIp;
    }

    void Reset(Word ip)
    {
        _csrf.Reset();
        _ip = ip;
    }

    std::optional<CpuToHostData> GetMessage()
    {
        return _csrf.GetMessage();
    }

private:
    Reg32 _ip;
    Decoder _decoder;
    RegisterFile _rf;
    CsrFile _csrf;
    Executor _exe;
    Memory &_mem;
};

#endif //RISCV_SIM_CPU_H
