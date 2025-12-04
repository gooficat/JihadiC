#pragma once

#include "data.h"

void InsOp_Direct(Instruction ptr instruction, ByteArray ptr output);

extern Register registers[];
extern u8 num_registers;

extern InstructionDescriptor instructions[];
extern u8 num_instruction;
