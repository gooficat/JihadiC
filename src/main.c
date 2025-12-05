#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "specification.h"
#include "token.h"

struc_t
{
   u64 location;
   u16 label_index;
   u8 addr_length;
}
LabelMarker;

ArrayType(LabelMarker) LabelMarkerArray;

struc_t
{
   Instruction ptr data;
   u64 length;
   StringArray labels;
   LabelMarkerArray label_markers;
}
AssemblyUnit;

String MakeString(char ptr initializer)
{
   String out;
   out.data = initializer;
   out.length = strlen(initializer);
   return out;
}

void ExtractImmediate(String ptr in, Instruction ptr out)
{
   out->profile[out->length] = TK_IMM;
   sscanf_s(in->data, "0x%hhx", addr(out->contents[out->length]));
   out->length++;
}

Instruction ParseExpression(StringArray ptr in)
{
   Instruction out = {};
   strcpy_s(out.mnemonic, 5, in->data[0].data);

   for (u64 i = 0; i < in->length; i++)
   {
      if (isalpha(in->data[i].data[0]))
      {
         int j;
         for (j = 0; j < num_registers; j++)
         {
            if (!strcmp(in->data[i].data, registers[j].name))
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
               if (!strcmp(in->data[i].data, instructions[k].name))
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
         ExtractImmediate(addr in->data[i], addr out);
      }
   }

   for (int i = 0; i < out.length; i++)
   {
      printf_s("Profile at %d is %d\n", i, out.profile[i]);
   }

   return out;
}

void TranslateInstruction(Instruction ptr instruction, ByteArray ptr output)
{
   for (int i = 0; i < num_instruction; i++)
   {
      if (strcmp(instruction->mnemonic, instructions[i].name))
         continue;
      printf_s("Checking instruction %s for match!\n", instructions[i].name);
      bool found = true;
      for (int j = 0; j < PROFILE_MAX; j++)
      {
         printf_s("Checking profile.\n");
         if (instruction->profile[j] != instructions[i].profile[j])
         {
            found = false;
            break;
         }
      }
      if (!found)
      {
         printf_s("No match between %s with first profile %hhu, and instruction of length %hhx "
                  "with first profile %hhu and mnemonic %s\n",
                  instructions[i].name, instructions[i].profile[0], instruction->length,
                  instruction->profile[0], instruction->mnemonic);
         continue;
      }

      instructions[i].operator(instruction, output);
      printf_s("Instruction matches with profile for %s! Output length is now %llu\n",
               instructions[i].name, output->length);
      break;
   }
}

#define FetchUntil(requirement, buffer)                                                            \
   while (requirement)                                                                             \
   {                                                                                               \
      ResizeArray(buffer, buffer.length + 1, char);                                                \
      buffer.data[buffer.length - 1] = (char)character;                                            \
      character = fgetc(in_file);                                                                  \
   }

AssemblyUnit AssembleInstructions(FILE ptr in_file)
{
   AssemblyUnit output = {.data = malloc(1),
                          .label_markers = {.data = malloc(1), .length = 0},
                          .labels = {.data = malloc(1), .length = 0},
                          .length = 0};

   String buffer = {.data = malloc(1), .length = 0};

   StringArray expression = {.data = malloc(1), .length = 0};

   bool finished = false;
#define next_token FetchToken(in_file, &buffer)
   if (!next_token)
   {
      finished = true;
   }
   while (!finished)
   {
      printf_s("|%s|\n", buffer.data);

      ResizeArray(expression, 1, String);
      expression.data[0] = buffer;
      buffer = (String){.data = malloc(1), .length = 0};

      while (next_token)
      {
         ResizeArray(expression, expression.length + 1, String);
         expression.data[expression.length - 1] = buffer;
         for (u64 p = 0; p < expression.length; p++)
         {
            printf_s("expression contains %s\n", expression.data[p].data);
         }
         buffer = MakeString(calloc(1, 1));

         if (!next_token)
            finished = true;

         if (buffer.data[0] != ',')
            break;
      }
      ResizeArray(output, output.length + 1, Instruction);
      output.data[output.length - 1] = ParseExpression(addr expression);
      printf_s("Reached end of an instruction\n");
   }
   free(buffer.data);

   return output;
}

int main(void)
{

   FILE ptr in_file;
   fopen_s(addr in_file, "../jc/jc-asm/test.as", "rt");

   // Instruction instruction = ParseExpression(expression, sizeof(expression) / sizeof(String));

   AssemblyUnit assembly_instructions = AssembleInstructions(in_file);

   ByteArray output = {.data = malloc(1), .length = 0};

   for (u64 i = 0; i < assembly_instructions.length; i++)
   {
      TranslateInstruction(addr assembly_instructions.data[i], addr output);
   }

   for (u64 i = 0; i < output.length; i++)
   {
      printf_s("0x%hhx\n", output.data[i]);
   }

   return 0;
}
