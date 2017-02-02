#pragma once
#include "tree.h"
#include "token.h"
#include "operator.h"

void tokenizer_initialize();

//value type tokens have to be freed
Token** tokenizer_process(char*);

void tokenizer_cleanup();