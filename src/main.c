#include <ctype.h>
#include <stdbool.h>
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
         int j;
         for (j = 0; j < num_registers; j++)
         {
            if (!strcmp(in[i].data, registers[j].name))
            {
               out.profile[out.length] = TK_REG;
               out.contents[out.length] = registers[j].opcode;
               out.length++;
               break;
            }
         }
         if (j == num_registers)
         {
            out.profile[out.length] = TK_INS;

            // i need a way to outsource the finding of the opcode in a way that allows me to still
            // have labels with alphanumeric names

            out.length++;
         }
      }
      else
      {
         out.profile[out.length] = TK_IMM;
         sscanf_s(in[i].data, "%hhu", &out.contents[out.length]);
         out.length++;
      }
   }

   return out;
}

int main(void)
{
   String expression[] = {MakeString("ret")};

   Instruction instruction = ParseExpression(expression);

   for (int i = 0; i < num_instruction; i++)
   {
      bool found = true;
      for (int j = 0; j < PROFILE_MAX; j++)
      {
         if ((!instructions[i].profile[j] && j < instruction.length - 1) ||
             instruction.profile[j] != instructions[i].profile[j])
         {
            found = false;
            break;
         }
      }
      if (!found)
         continue;

      printf_s("Instruction matches with profile!\n");
   }

   return 0;
}
