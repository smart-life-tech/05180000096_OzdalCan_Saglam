#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TextJedi.h"
#include "common.h"
#include "tokenizer.h"

char *emit_token(tokenizer_state_t *state, char *lexeme, int line, int pos)
{
  token_t *token = NULL;

  if (tokens == NULL)
  {
    tokens = malloc(sizeof(token_t));
    token_count = 1;
    token = tokens;
  }
  else
  {
    tokens = realloc(tokens, (token_count + 1) * sizeof(token_t));
    token = &tokens[token_count];
    token_count++;
  }

  token->lexeme = strdup(lexeme);

  switch (*state)
  {
  case CONSUME_IDENTIFIER_OR_KEYWORD:
  {
    int is_keyword = check_is_keyword(lexeme);

    if (is_keyword == 1)
    {
      token->type = KEYWORD;
    }
    else
    {
      token->type = IDENTIFIER;

      // validate identifier
      if (strlen(lexeme) > 30)
      {
        sprintf(errbuf, "IdentifierTooLong: max identifier size(30) exceeded. Invalid identifier '%s'", lexeme);
        fatal(errbuf, line, pos);
      }

      if (lexeme[0] == '_')
      {
        fatal("InvalidIdentifier: identifiers cannot start with '_'", line, pos);
      }
    }

    break;
  }
  case CONSUME_OPERATOR:
  {
    if (strcmp(lexeme, "+") == 0)
    {
      token->type = OP_PLUS;
    }
    else if (strcmp(lexeme, "-") == 0)
    {
      token->type = OP_MINUS;
    }
    else if (strcmp(lexeme, ":=") == 0)
    {
      token->type = OP_ASSIGN;
    }
    break;
  }
  case CONSUME_NUMBER_LITERAL:
    token->type = NUMBER_LITERAL;

    long long ival = atoll(lexeme);

    if (ival > UINT32_MAX)
    {
      sprintf(errbuf, "InvalidNumberLiteral: Numbers cannot be greater than UINT32_MAX (%lld)", (long long)UINT32_MAX);
      fatal(errbuf, line, pos);
    }

    break;

  case CONSUME_STRING_LITERAL:
    token->type = STRING_LITERAL;
    token->lexeme = strdup(lexeme);
    break;

  default:
    sprintf(errbuf, "emit_token: unexpected tokenizer state: %s", tokenizer_state_names[*state]);
    fatal(errbuf, line, pos);
  }

  printf("emitted token: %s, type: %s, line: %d, pos: %d\n", token->lexeme, token_type_names[token->type], line, pos);

  *state = DETECT_TOKEN_TYPE;

  free(lexeme);

  return NULL;
}

void tokenize()
{
  char *cursor = program;
  char *lexeme = NULL;
  int is_consuming = 0;
  int line = 1;
  int pos = 1;
  tokenizer_state_t state = DETECT_TOKEN_TYPE;
  int tokenstartline = line;
  int tokenstartpos = pos;

  for (char c = *cursor; c != '\0'; cursor++, pos++, c = *cursor)
  {
    switch (c)
    {
    case '\n':
      line++;
      pos = 1;
    case ';':
    case ' ':
      if (state == DETECT_TOKEN_TYPE)
      {
        continue;
      }
    case ':':
    case '+':
    case '-':
    case '=':
      if ((state == DETECT_TOKEN_TYPE && (c == ':' || c == '+' || c == '-')) || (state == CONSUME_OPERATOR && c == '='))
      {
        state = CONSUME_OPERATOR;
        lexeme = add_char(lexeme, c);

        if (c == ':')
        {
          // this character must be proceeded with '='
          continue;
        }
      }

      if (state == CONSUME_IDENTIFIER_OR_KEYWORD ||
          state == CONSUME_NUMBER_LITERAL ||
          // only the assignment operator will reach here
          state == CONSUME_OPERATOR)
      {
        lexeme = emit_token(&state, lexeme, tokenstartline, tokenstartpos);
        continue;
      }
    case '_':
    case 'A' ... 'Z':
    case 'a' ... 'z':
      if (state == DETECT_TOKEN_TYPE)
      {
        state = CONSUME_IDENTIFIER_OR_KEYWORD;
        tokenstartline = line;
        tokenstartpos = pos;
      }
    case '0' ... '9':
      if (state == DETECT_TOKEN_TYPE)
      {
        state = CONSUME_NUMBER_LITERAL;
        tokenstartline = line;
        tokenstartpos = pos;
      }

      lexeme = add_char(lexeme, c);

      break;

    case '"':
      if (state == DETECT_TOKEN_TYPE)
      {
        state = CONSUME_STRING_LITERAL;
      }
      else if (state == CONSUME_STRING_LITERAL)
      {
        lexeme = emit_token(&state, lexeme, tokenstartline, tokenstartpos);
      }
      else
      {
        fatal("Unexpected character '\"'", line, pos);
      }

      break;

    default:
      sprintf(errbuf, "Invalid character '%c'", c);
      fatal(errbuf, line, pos);
    }
  }

  if (state != DETECT_TOKEN_TYPE)
  {
    printf("Unexpected EOF at line %d pos %d", line, pos);

    if (state == CONSUME_IDENTIFIER_OR_KEYWORD || state == CONSUME_NUMBER_LITERAL)
    {
      printf(": Expected end of statement ';'.");
    }
    if (state == CONSUME_STRING_LITERAL)
    {
      printf(": Expected string terminator.");
    }

    printf("\n");
  }
}
