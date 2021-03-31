
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <vector>
#include <functional>
#include <map>
class Executor
{
public:
    void Execute(InstructionPtr &instr, Word ip)
    {
        Word arg2;
        if (instr->_imm)
            arg2 = instr->_imm.value();
        else
            arg2 = instr->_src2Val;
        Word result = _alu_funcs[instr->_aluFunc](instr->_src1Val, arg2);
        instr->_data = result;
        instr->_nextIp = ip + 4;
        switch (instr->_type)
        {
        case IType::Alu:
            break;
        case IType::Auipc:
            instr->_data = ip + instr->_imm.value();
            break;
        case IType::Br:
            if (_br_funcs[instr->_brFunc](instr->_src1Val, instr->_src2Val))
                instr->_nextIp = ip + instr->_imm.value();
            break;
        case IType::Csrr:
            instr->_data = instr->_csrVal;
            break;
        case IType::Csrw:
            instr->_data = instr->_src1Val;
            break;
        case IType::J:
            instr->_data = ip + 4;
            if (_br_funcs[instr->_brFunc](instr->_src1Val, instr->_src2Val))
                instr->_nextIp = ip + instr->_imm.value();
            break;
        case IType::Jr:
            instr->_data = ip + 4;
            if (_br_funcs[instr->_brFunc](instr->_src1Val, instr->_src2Val))
                instr->_nextIp = instr->_imm.value() + instr->_src1Val;
            break;
        case IType::Ld:
            instr->_addr = result;
            break;
        case IType::St:
            instr->_addr = result;
            instr->_data = instr->_src2Val;
            break;
        }
    }
    
    Executor()
    {
        _alu_funcs[AluFunc::Add] = [this](Word arg1, Word arg2) { return (int)arg1 + (int)arg2; };
        _alu_funcs[AluFunc::Sub] = [this](Word arg1, Word arg2) { return (int)arg1 - (int)arg2; };
        _alu_funcs[AluFunc::And] = [this](Word arg1, Word arg2) { return arg1 & arg2; };
        _alu_funcs[AluFunc::Or] = [this](Word arg1, Word arg2) { return arg1 | arg2; };
        _alu_funcs[AluFunc::Xor] = [this](Word arg1, Word arg2) { return arg1 ^ arg2; };
        _alu_funcs[AluFunc::Slt] = [this](Word arg1, Word arg2) { return ((int)arg1 < (int)arg2); };
        _alu_funcs[AluFunc::Sltu] = [this](Word arg1, Word arg2) { return ((uint32_t)arg1 < (uint32_t)arg2); };
        _alu_funcs[AluFunc::Sll] = [this](Word arg1, Word arg2) { return (int)arg1 << (int)(arg2 % 32); };
        _alu_funcs[AluFunc::Srl] = [this](Word arg1, Word arg2) { return (uint32_t)arg1 >> (uint32_t)(arg2 % 32); };
        _alu_funcs[AluFunc::Sra] = [this](Word arg1, Word arg2) { return ((int)arg1 >> (int)(arg2 % 32)); };

        _br_funcs[BrFunc::Eq] = [this](Word arg1, Word arg2) { return ((int)arg1 == (int)arg2); };
        _br_funcs[BrFunc::Neq] = [this](Word arg1, Word arg2) { return ((int)arg1 != (int)arg2); };
        _br_funcs[BrFunc::Lt] = [this](Word arg1, Word arg2) { return ((int)arg1 < (int)arg2); };
        _br_funcs[BrFunc::Ltu] = [this](Word arg1, Word arg2) { return ((uint32_t)arg1 < (uint32_t)arg2); };
        _br_funcs[BrFunc::Ge] = [this](Word arg1, Word arg2) { return ((int)arg1 >= (int)arg2); };
        _br_funcs[BrFunc::Geu] = [this](Word arg1, Word arg2) { return ((uint32_t)arg1 >= (uint32_t)arg2); };
        _br_funcs[BrFunc::AT] = [this](Word arg1, Word arg2) { return true; };
        _br_funcs[BrFunc::NT] = [this](Word arg1, Word arg2) { return false; };
    }

private:
    std::map<AluFunc, std::function<Word(Word, Word)>> _alu_funcs;
    std::map<BrFunc, std::function<Word(Word, Word)>> _br_funcs;
};

#endif // RISCV_SIM_EXECUTOR_H
