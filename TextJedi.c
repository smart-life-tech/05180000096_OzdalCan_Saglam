#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256
#define MAX_LINE_SIZE 1024

typedef struct
{
    enum
    {
        TOKEN_KEYWORD,
        TOKEN_IDENTIFIER,
        TOKEN_STRING,
        TOKEN_INTEGER,
        TOKEN_END_OF_LINE
    } type;
    char lexeme[MAX_BUFFER_SIZE];
} Token;

Token getNextToken(FILE *file)
{
    Token token;
    char buffer[MAX_BUFFER_SIZE];

    if (fscanf(file, "%s", buffer) == 1)
    {
        if (strcmp(buffer, "new") == 0)
        {
            token.type = TOKEN_KEYWORD;
            strcpy(token.lexeme, buffer);
        }
        else if (strcmp(buffer, "text") == 0 || strcmp(buffer, "int") == 0)
        {
            token.type = TOKEN_KEYWORD;
            strcpy(token.lexeme, buffer);
        }
        else if (strcmp(buffer, "output") == 0 || strcmp(buffer, "asString") == 0)
        {
            token.type = TOKEN_KEYWORD;
            strcpy(token.lexeme, buffer);
        }
        else if (strcmp(buffer, ":=") == 0)
        {
            token.type = TOKEN_KEYWORD;
            strcpy(token.lexeme, buffer);
        }
        else if (strcmp(buffer, ";") == 0)
        {
            token.type = TOKEN_END_OF_LINE;
            strcpy(token.lexeme, buffer);
        }
        else if (buffer[0] == '"')
        {
            token.type = TOKEN_STRING;
            strcpy(token.lexeme, buffer);
        }
        else if (atoi(buffer) != 0 || strcmp(buffer, "0") == 0)
        {
            token.type = TOKEN_INTEGER;
            strcpy(token.lexeme, buffer);
        }
        else
        {
            token.type = TOKEN_IDENTIFIER;
            strcpy(token.lexeme, buffer);
        }
    }
    else
    {
        token.type = TOKEN_END_OF_LINE;
        strcpy(token.lexeme, "");
    }

    return token;
}

void executeTextJediProgram(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[MAX_LINE_SIZE];
    char myString[MAX_BUFFER_SIZE];
    int myNumber;

    while (fgets(line, sizeof(line), file))
    {
        // Remove the newline character from the line
        line[strcspn(line, "\n")] = '\0';

        char *token = strtok(line, " ");

        if (token != NULL)
        {
            if (strcmp(token, "new") == 0)
            {
                // Process new variable declaration
                token = strtok(NULL, " ");         // Get variable type
                char *varName = strtok(NULL, " "); // Get variable name

                if (token != NULL && varName != NULL)
                {
                    if (strcmp(token, "text") == 0)
                    {
                        // Text variable
                        strcpy(myString, "");
                        printf("New text variable created: %s\n", varName);
                    }
                    else if (strcmp(token, "int") == 0)
                    {
                        // Integer variable
                        myNumber = 0;
                        printf("New int variable created: %s\n", varName);
                    }
                    else
                    {
                        printf("Syntax Error: Unknown variable type '%s'\n", token);
                    }
                }
                else
                {
                    printf("Syntax Error: Expected variable type and name after 'new'\n");
                }
            }
            else if (strcmp(token, "myString") == 0)
            {
                // Process myString assignment
                token = strtok(NULL, " "); // Get assignment operator

                if (token != NULL && strcmp(token, ":=") == 0)
                {
                    token = strtok(NULL, "\""); // Get string value between quotation marks

                    if (token != NULL)
                    {
                        strcpy(myString, token);
                    }
                    else
                    {
                        printf("Syntax Error: Expected string value after ':='\n");
                    }
                }
                else
                {
                    printf("Syntax Error: Expected ':=' after variable name\n");
                }
            }
            else if (strcmp(token, "output") == 0)
            {
                // Process output operation
                printf("Output: %s\n", myString);
            }
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <program_file>\n", argv[0]);
        return 1;
    }

    const char *programFile = argv[1];
    executeTextJediProgram(programFile);

    return 0;
}
