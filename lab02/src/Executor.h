
#ifndef RISCV_SIM_EXECUTOR_H
#define RISCV_SIM_EXECUTOR_H

#include "Instruction.h"
#include <vector>
#include <functional>
#include <map>
class Executor
{
public:
    void Execute(InstructionPtr& instr, Word ip)
    {
        if (instr->_src1.has_value() && (instr->_imm.has_value() || instr->_src2.has_value()))
        {  
            Word op1 = instr->_src1Val;        
            Word op2 = instr->_imm.value_or(instr->_src2Val);
            Word result = _alu_funcs[instr->_aluFunc](op1, op2);  

            switch (instr->_type) 
            {
                case IType::Alu:
                    instr->_data = result; 
                    break;
                case IType::St:
                    instr->_data = instr->_src2Val; 
                    instr->_addr = result;  
                    break;
                case IType::Ld:
                    instr->_addr = result;  
                    break;
            }
        }
        switch (instr->_type) 
        {
                case IType::Csrr:
                    instr->_data = instr->_csrVal;  
                    break;
                case IType::Csrw:
                    instr->_data = instr->_src1Val;  
                    break;
                case IType::J:
                case IType::Jr:
                    instr->_data = ip + 4; 
                    break;
                case IType::Auipc:
                    instr->_data = ip + instr->_imm.value(); 
                    break;
        }
        // branching block
        Word next_addr;
        bool branch = false;
        if (instr->_src1.has_value() && instr->_src2.has_value())
        {
            switch (instr->_type)
            {
                case IType::Br:
                case IType::J:
                    next_addr = ip + instr->_imm.value();
                    break;
                case IType::Jr:
                    next_addr = instr->_src1Val + instr->_imm.value();
                    break;
            }
            Word op1 = instr->_src1Val;
            Word op2 = instr->_src2Val;
            branch = _br_funcs[instr->_brFunc](op1, op2);
        }
        if (branch)
            instr->_nextIp = next_addr;
        else
            instr->_nextIp = ip + 4;
    }

    Executor()
    {
        _alu_funcs[AluFunc::Add] = [this](Word op1, Word op2) { return op1 + op2; };
        _alu_funcs[AluFunc::Sll] = [this](Word op1, Word op2) { return op1 << (op2 % 32); };
        _alu_funcs[AluFunc::Slt] = [this](Word op1, Word op2) { return (Word)((int16_t)op1 < (int16_t)op2); };
        _alu_funcs[AluFunc::Sltu] = [this](Word op1, Word op2) { return (Word)(op1 < op2); };
        _alu_funcs[AluFunc::Xor] = [this](Word op1, Word op2) { return op1 ^ op2; };
        _alu_funcs[AluFunc::And] = [this](Word op1, Word op2) { return op1 & op2; };
        _alu_funcs[AluFunc::Or] = [this](Word op1, Word op2) { return op1 | op2; };
        _alu_funcs[AluFunc::Sub] = [this](Word op1, Word op2) { return op1 - op2; };
        _alu_funcs[AluFunc::Sra] = [this](Word op1, Word op2) { return (Word)((int16_t)op1 >> (op2 % 32)); };
        _alu_funcs[AluFunc::Srl] = [this](Word op1, Word op2) { return op1 >> (op2 % 32); };
        _br_funcs[BrFunc::Eq] = [this](Word op1, Word op2) { return (Word)(op1 == op2); };
        _br_funcs[BrFunc::Neq] = [this](Word op1, Word op2) { return (Word)(op1 != op2); };
        _br_funcs[BrFunc::Lt] = [this](Word op1, Word op2) { return (Word)((int16_t)op1 < (int16_t)op2); };
        _br_funcs[BrFunc::Ltu] = [this](Word op1, Word op2) { return (Word)(op1 < op2); };
        _br_funcs[BrFunc::Ge] = [this](Word op1, Word op2) { return (Word)((int16_t)op1 >= (int16_t)op2); };
        _br_funcs[BrFunc::Geu] = [this](Word op1, Word op2) { return (Word)(op1 >= op2); };
        _br_funcs[BrFunc::AT] = [this](Word op1, Word op2) { return 1; };
        _br_funcs[BrFunc::NT] = [this](Word op1, Word op2) { return 0; };
    }

private:
    std::map<AluFunc, std::function<Word(Word, Word)>> _alu_funcs;
    std::map<BrFunc, std::function<Word(Word, Word)>> _br_funcs;
};

#endif // RISCV_SIM_EXECUTOR_H
