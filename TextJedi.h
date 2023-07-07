#pragma once

typedef enum
{
  STRING,
  INTEGER
} variable_type_t;

typedef struct
{
  variable_type_t type;
  char name[50];
  char *strval;
  int ival;
} variable_t;

typedef struct expression_t
{
  char *strval;
  int ival;
  struct expression_t *kids;
  int kid_count;
} expression_t;

typedef enum
{
  VARIABLE_DEFINITION,
  FUNCTION_CALL,
} statement_type_t;

typedef struct
{
  statement_type_t type;
  expression_t expressions;
} statement_t;

// [START] Tokeniers

typedef enum
{
  IDENTIFIER = 0,   // abc123
  STRING_LITERAL,   // "..."
  NUMBER_LITERAL,   // 123
  OP_PLUS,          // +
  OP_MINUS,         // -
  OP_ASSIGN,        // :=
  OP_STATEMENT_END, // ;
  KEYWORD,          // new, text
} token_type_t;

static volatile const char *token_type_names[] = {
    "IDENTIFIER",
    "STRING_LITERAL",
    "NUMBER_LITERAL",
    "OP_PLUS",
    "OP_MINUS",
    "OP_ASSIGN",
    "OP_STATEMENT_END",
    "KEYWORD",
};

typedef struct
{
  token_type_t type;
  char *lexeme;
} token_t;
// [END] Tokenizer

// [START] Parser State
typedef enum
{
  EMPTY
} parser_state_t;
// [END] Parser State

// Constants
extern char *program;
extern token_t *tokens;
extern int token_count;
