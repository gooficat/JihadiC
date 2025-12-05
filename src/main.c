#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "specification.h"

String MakeString(char ptr initializer)
{
   String out;
   out.data = initializer;
   out.length = strlen(initializer);
   return out;
}

Instruction ParseExpression(String ptr in, u8 length)
{
   Instruction out = {};

   for (u64 i = 0; i < length; i++)
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

            // I need a way to outsource the finding of the opcode in a way that allows me to still
            // have labels with alphanumeric names. This solution here will not work if a mnemonic
            // has multiple ops attached to it.
            out.profile[out.length] = TK_INS;
            for (int k = 0; k < num_instruction; k++)
            {
               if (!strcmp(in[i].data, instructions[k].name))
               {
                  out.contents[out.length] = instructions[k].opcode;
                  break;
               }
            }
            out.length++;
         }
      }
      else
      {
         out.profile[out.length] = TK_IMM;
         sscanf_s(in[i].data, "0x%hhx", &out.contents[out.length]);
         out.length++;
      }
   }

   for (int i = 0; i < out.length; i++)
   {
      printf_s("Profile at %d is %d\n", i, out.profile[i]);
   }

   return out;
}

int main(void)
{
   String expression[] = {MakeString("byt"), MakeString("0x54")};

   Instruction instruction = ParseExpression(expression, sizeof(expression) / sizeof(String));

   ByteArray output = {.data = malloc(1), .length = 0};

   for (int i = 0; i < num_instruction; i++)
   {
      printf_s("Checking instruction %s for match!\n", instructions[i].name);
      bool found = true;
      for (int j = 0; j < PROFILE_MAX; j++)
      {
         printf_s("Checking profile.\n");
         if (instruction.profile[j] != instructions[i].profile[j])
         {
            found = false;
            break;
         }
      }
      if (!found)
      {
         printf_s("No match between %s with first profile %hhu, and instruction of length %hhx "
                  "with first profile %hhu\n",
                  instructions[i].name, instructions[i].profile[0], instruction.length,
                  instruction.profile[0]);
         continue;
      }

      instructions[i].operator(addr instruction, addr output);
      printf_s("Instruction matches with profile for %s! Output length is now %llu\n",
               instructions[i].name, output.length);
      break;
   }

   for (u64 i = 0; i < output.length; i++)
   {
      printf_s("0x%hhx\n", output.data[i]);
   }

   return 0;
}
