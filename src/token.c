#include "token.h"
#include <ctype.h>

int FetchToken(FILE ptr input, String ptr token)
{
   int character = fgetc(input);
   token->length = 0;
   if (character != EOF)
   {
      while (isspace(character))
      {
         character = fgetc(input);
         if (character == EOF)
            return 0;
      }
      if (isalnum(character))
      {
         do
         {
            ResizeArray(*token, token->length + 1, char);
            token->data[token->length - 1] = (char)character;
            character = fgetc(input);
         } while (isalnum(character));
         ungetc(character, input);
         ResizeArray(*token, token->length + 1, char);
         token->data[token->length - 1] = 0;
      }
      else
      {
         ResizeArray(*token, 2, char);
         token->data[0] = (char)character;
         token->data[1] = 0;
      }

      return 1;
   }
   else
   {
      return 0;
   }
}