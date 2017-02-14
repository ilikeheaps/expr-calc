#pragma once
typedef enum
{
    operator,
    value,
    openBracket,
    endBracket,
    valSep
} atom;

typedef struct
{
    atom type;
    void* value;
} Token;

Token* newToken(atom, void*);

Token* copyToken(Token*);