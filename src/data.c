#include "data.h"
#include <stdlib.h>

void ResizeArrayFunc(struct GenericArray ptr array, u64 new_size)
{
   array->data = realloc(array->data, new_size);
}