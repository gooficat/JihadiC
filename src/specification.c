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

Register registers[] = {{"ax", 0x00}, {"cx", 0x01}, {"dx", 0x02}, {"bx", 0x03},
                        {"sp", 0x04}, {"bp", 0x05}, {"si", 0x06}, {"di", 0x07}};
u8 num_registers = sizeof(registers) / sizeof(Register);

InstructionDescriptor instructions[] = {{"ret", 0xC3, {TK_INS}, InsOp_Direct}};
u8 num_instruction = sizeof(instructions) / sizeof(InstructionDescriptor);
