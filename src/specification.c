#include "specification.h"
#include <string.h>
#include "data.h"

void InsOp_Direct(Instruction ptr instruction, ByteArray ptr output)
{
   u64 initial_index = output->length;
   ResizeArray(*output, output->length + instruction->length, u8);
   memcpy_s(output->data + initial_index, instruction->length, instruction->contents,
            instruction->length);
}

void InsOp_AddFirst2(Instruction ptr instruction, ByteArray ptr output)
{
   u64 initial_index = output->length;
   ResizeArray(*output, output->length + instruction->length - 1, u8);
   output->data[initial_index] = instruction->contents[0] + instruction->contents[1];
   if (instruction->length <= 2)
      return;
   memcpy_s(output->data + initial_index + 1, instruction->length - 2, instruction->contents + 2,
            instruction->length - 2);
}

Register registers[] = {
    {"ax", 0x00}, {"cx", 0x01}, {"dx", 0x02}, {"bx", 0x03}, {"sp", 0x04}, {"bp", 0x05},
    {"si", 0x06}, {"di", 0x07}, {"al", 0x00}, {"cl", 0x01}, {"dl", 0x02}, {"bl", 0x03},
    {"ah", 0x04}, {"ch", 0x05}, {"dh", 0x06}, {"bh", 0x07},
};
u8 num_registers = sizeof(registers) / sizeof(Register);

InstructionDescriptor instructions[] = {{"ret", 0xC3, {TK_INS}, InsOp_Direct},
                                        {"mov", 0x0B, {TK_INS, TK_REG, TK_IMM}, InsOp_AddFirst2},
                                        {"jmp", 0xEA, {TK_INS, TK_IMM}, InsOp_AddFirst2},
                                        {"byt", 0x00, {TK_INS, TK_IMM}, InsOp_AddFirst2},
                                        {"int", 0xDC, {TK_INS, TK_IMM}, InsOp_Direct}};
u8 num_instruction = sizeof(instructions) / sizeof(InstructionDescriptor);
