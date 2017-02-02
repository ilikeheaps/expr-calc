#include "token.h"
#include <stdlib.h>

Token* newToken(atom type, void* value)
{
    Token* new = malloc(sizeof(*new));
    new->type = type;
    new->value = value;
    return new;
}

Token* copyToken(Token* orig)
{
    return newToken(orig -> type, orig -> value);
}
    