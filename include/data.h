#pragma once
#include "defs.h"

enu_m{TK_EMP, TK_INS, TK_IMM, TK_REG} TokenType;

#define PROFILE_MAX 4

struct GenericArray
{
   void ptr data;
   u64 length;
};

void ResizeArrayFunc(struct GenericArray ptr array, u64 new_size);

#define ResizeArray(array, new_size, element_type)                                                 \
   ResizeArrayFunc((struct GenericArray ptr)(addr array), new_size * sizeof(element_type))

#define ArrayType(type)                                                                            \
   struc_t                                                                                         \
   {                                                                                               \
      type ptr data;                                                                               \
      u64 length;                                                                                  \
   }

ArrayType(u8) ByteArray;

ArrayType(char) String;

ArrayType(String) StringArray;

struc_t
{
   char mnemonic[5];
   u8 contents[4];
   TokenType profile[PROFILE_MAX];
   u8 length;
}
Instruction;

typedef void(ptr InstructionOperator)(Instruction ptr instruction, ByteArray ptr output);

struc_t
{
   char name[5];
   u8 opcode;
   TokenType profile[PROFILE_MAX];
   InstructionOperator operator;
}
InstructionDescriptor;

struc_t
{
   char name[4];
   u8 opcode;
}
Register;
