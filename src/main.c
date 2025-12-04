#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "specification.h"

struc_t
{
   char ptr data;
   u64 length;
}
String;

String MakeString(char ptr initializer)
{
   String out;
   out.data = initializer;
   out.length = strlen(initializer);
   return out;
}

Instruction ParseExpression(String ptr in)
{
   Instruction out;
   out.length = 0;

   for (u64 i = 0; i < in->length; i++)
   {
      if (isalpha(in[i].data[0]))
      {
         for (int j = 0; j < num_registers; j++)
         {
            if (!strcmp(in[i].data, registers[j].name))
            {
               out.profile[out.length] = TK_REG;
               out.contents[out.length] = registers[j].opcode;
               out.length++;
               break;
            }
         }
      }
      else
      {
         out.profile[out.length] = TK_IMM;
         sscanf_s(in[i].data, "%hhu", out.contents[out.length]);
         out.length++;
      }
   }

   return out;
}

int main(void)
{
   String expression[] = {MakeString("ax"), MakeString("234")};

   Instruction instruction = ParseExpression(expression);

   for (int i = 0; i < instruction.length; i++)
   {
      printf_s("0x%hhX\n", instruction.contents[i]);
   }

   return 0;
}
