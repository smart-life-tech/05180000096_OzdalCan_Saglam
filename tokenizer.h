#pragma once

typedef enum
{
  DETECT_TOKEN_TYPE = 0,
  CONSUME_IDENTIFIER_OR_KEYWORD,
  CONSUME_NUMBER_LITERAL,
  CONSUME_STRING_LITERAL,
  CONSUME_OPERATOR
} tokenizer_state_t;

static volatile const char *tokenizer_state_names[] = {
    "DETECT_TOKEN_TYPE",
    "CONSUME_IDENTIFIER_OR_KEYWORD",
    "CONSUME_NUMBER_LITERAL",
    "CONSUME_STRING_LITERAL",
    "CONSUME_OPERATOR",
};

void tokenize();
