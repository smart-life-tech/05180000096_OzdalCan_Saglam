#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "TextJedi.h"
#include "tokenizer.h"
#include "common.h"

char *program = "new text fatih;\n"
                "new int numara;\n"
                "fatih := 1234;\n"
                "fatih := 123 + 123;\n"
                "fatih := 123 - 1234;\n"
                "fatih := \"Fatih Kaya\"\n";

token_t *tokens;
int token_count;

variable_t *variables;
int variable_count;

void init()
{
  variable_count = 0;
  variables = NULL;
}

void add_variable(char *name, variable_type_t type, char *strval, int ival)
{
  int new_variable_index = variable_count;

  variables = realloc(variables, (variable_count + 1) * sizeof(variable_t));

  variable_t var = variables[new_variable_index];

  // set variable type
  var.type = type;

  // set variable name
  memcpy(var.name, name, strlen(name) + 1);

  // set variable's value
  if (type == STRING)
  {
    var.strval = strdup(strval);
    var.ival = 0;
  }
  else
  {
    var.strval = NULL;
    var.ival = ival;
  }
}

void parse()
{
}

int main(int argc, char *argv[])
{
  init();

  tokenize();

  return 0;
}
