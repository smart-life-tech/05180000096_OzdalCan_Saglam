#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDENTIFIER_SIZE 30
#define MAX_INTEGER_SIZE 10
#define MAX_STRING_SIZE 1000
#define MAX_BUFFER_SIZE 100
#define MAX_LINE_SIZE 1000
#define MAX_LEXEME_SIZE 1000
typedef enum
{
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_KEYWORD,
    TOKEN_END_OF_LINE,
    TOKEN_COMMENT,
    TOKEN_ERROR
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[MAX_IDENTIFIER_SIZE + 1];
} Token;

int isKeyword(const char *identifier)
{
    const char *keywords[] = {
        "new", "int", "text", "size", "subs", "locate", "insert", "override",
        "read", "write", "from", "to", "input", "output", "asText", "asString"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < numKeywords; ++i)
    {
        if (strcmp(identifier, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

Token getNextTokenFromLine(const char *line, int *position)
{
    Token token;
    const int lineLength = strlen(line);
    const int start = *position;

    // Skip whitespace characters
    while (*position < lineLength && isspace(line[*position]))
        (*position)++;

    // Check for end of line or end of string
    if (*position >= lineLength || line[*position] == '\0')
    {
        token.type = TOKEN_END_OF_LINE;
        strcpy(token.lexeme, ";"); // Set the end-of-line token lexeme to ";"
        return token;
    }

    // Handle identifier or keyword
    if (isalpha(line[*position]) || line[*position] == '_')
    {
        int i = 0;
        token.lexeme[i++] = line[*position];

        (*position)++;

        while (*position < lineLength && (isalnum(line[*position]) || line[*position] == '_'))
        {
            if (i < MAX_LEXEME_SIZE - 1)
                token.lexeme[i++] = line[*position];
            (*position)++;
        }

        token.lexeme[i] = '\0';

        if (isKeyword(token.lexeme))
            token.type = TOKEN_KEYWORD;
        else
            token.type = TOKEN_IDENTIFIER;

        return token;
    }

    // Handle integer constant
    if (isdigit(line[*position]))
    {
        int i = 0;
        while (*position < lineLength && isdigit(line[*position]))
        {
            if (i < MAX_LEXEME_SIZE - 1)
                token.lexeme[i++] = line[*position];
            (*position)++;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_INTEGER;
        return token;
    }

    // Handle string constant
    if (line[*position] == '"')
    {
        int i = 0;
        token.lexeme[i++] = line[*position];

        (*position)++;

        while (*position < lineLength && line[*position] != '\0' && line[*position] != '"')
        {
            if (i < MAX_LEXEME_SIZE - 1)
                token.lexeme[i++] = line[*position];
            (*position)++;
        }

        if (line[*position] == '"')
        {
            token.lexeme[i++] = line[*position];
            (*position)++;
            token.lexeme[i] = '\0';
            token.type = TOKEN_STRING;
        }
        else
        {
            token.lexeme[i] = '\0';
            token.type = TOKEN_ERROR; // Unterminated string constant
        }

        return token;
    }

    // Handle other cases: operators, etc.
    // ...

    // If none of the above cases matched, it's an error token
    token.type = TOKEN_ERROR;
    strcpy(token.lexeme, "ERROR");
    (*position) += strlen(token.lexeme); // Update the position
    return token;
}

void printToken(Token token)
{
    switch (token.type)
    {
    case TOKEN_IDENTIFIER:
        printf("Identifier: %s\n", token.lexeme);
        break;
    case TOKEN_INTEGER:
        printf("Integer: %s\n", token.lexeme);
        break;
    case TOKEN_STRING:
        printf("String: %s\n", token.lexeme);
        break;
    case TOKEN_OPERATOR:
        printf("Operator: %s\n", token.lexeme);
        break;
    case TOKEN_KEYWORD:
        printf("Keyword: %s\n", token.lexeme);
        break;
    case TOKEN_END_OF_LINE:
        printf("End of Line\n");
        break;
    case TOKEN_COMMENT:
        printf("Comment\n");
        break;
    case TOKEN_ERROR:
        printf("Lexical Error\n");
        break;
    default:
        break;
    }
}

Token getNextToken(FILE *file)
{
    Token token;

    token.lexeme[0] = '\0';

    int ch = fgetc(file);

    // Skip whitespaces
    while (isspace(ch))
    {
        ch = fgetc(file);
    }

    if (ch == EOF)
    {
        token.type = TOKEN_END_OF_LINE;
        return token;
    }

    if (isalpha(ch) || ch == '_')
    {
        // Identifier or keyword
        int i = 0;
        while (isalnum(ch) || ch == '_')
        {
            if (i < MAX_IDENTIFIER_SIZE)
            {
                token.lexeme[i++] = ch;
            }
            else
            {
                token.type = TOKEN_ERROR;
                return token;
            }
            ch = fgetc(file);
        }
        token.lexeme[i] = '\0';

        if (isKeyword(token.lexeme))
        {
            token.type = TOKEN_KEYWORD;
        }
        else
        {
            token.type = TOKEN_IDENTIFIER;
        }

        ungetc(ch, file);
    }
    else if (isdigit(ch))
    {
        // Integer constant
        int i = 0;
        while (isdigit(ch))
        {
            if (i < MAX_INTEGER_SIZE)
            {
                token.lexeme[i++] = ch;
            }
            else
            {
                token.type = TOKEN_ERROR;
                return token;
            }
            ch = fgetc(file);
        }
        token.lexeme[i] = '\0';

        token.type = TOKEN_INTEGER;
        ungetc(ch, file);
    }
    else if (ch == '"')
    {
        // String constant
        int i = 0;
        ch = fgetc(file);

        while (ch != '"' && ch != EOF)
        {
            if (i < MAX_STRING_SIZE)
            {
                token.lexeme[i++] = ch;
            }
            else
            {
                token.type = TOKEN_ERROR;
                return token;
            }
            ch = fgetc(file);
        }

        if (ch == EOF)
        {
            token.type = TOKEN_ERROR;
            return token;
        }

        token.lexeme[i] = '\0';
        token.type = TOKEN_STRING;
    }
    else if (ch == '+' || ch == '-' || ch == ':')
    {
        // Operator
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_OPERATOR;
    }
    else if (ch == ';')
    {
        // End of line
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        token.type = TOKEN_END_OF_LINE;
    }
    else if (ch == '/')
    {
        // Comment or operator
        token.lexeme[0] = ch;
        ch = fgetc(file);

        if (ch == '*')
        {
            // Comment
            int commentClosed = 0;
            while (!commentClosed && ch != EOF)
            {
                ch = fgetc(file);
                if (ch == '*')
                {
                    ch = fgetc(file);
                    if (ch == '/')
                    {
                        commentClosed = 1;
                    }
                }
            }

            if (!commentClosed)
            {
                token.type = TOKEN_ERROR;
                return token;
            }

            token.type = TOKEN_COMMENT;
        }
        else
        {
            // Operator
            ungetc(ch, file);
            token.lexeme[1] = '\0';
            token.type = TOKEN_OPERATOR;
        }
    }
    else
    {
        // Invalid character
        token.type = TOKEN_ERROR;
    }

    return token;
}

void readStringFromFile(const char *filename, char *output)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char ch;
    int i = 0;

    while ((ch = fgetc(file)) != EOF)
    {
        if (i < MAX_STRING_SIZE - 1)
        {
            output[i++] = ch;
        }
        else
        {
            printf("File content exceeds maximum string size.\n");
            break;
        }
    }

    output[i] = '\0';

    fclose(file);
}

void writeStringToFile(const char *filename, const char *content)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    fprintf(file, "%s", content);

    fclose(file);
}

void getInputFromUser(const char *prompt, char *input)
{
    printf("%s: ", prompt);
    fgets(input, MAX_BUFFER_SIZE, stdin);

    // Remove newline character if present
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
}

void outputText(const char *text)
{
    printf("%s\n", text);
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
    Token token;

    while (fgets(line, sizeof(line), file))
    {
        int position = 0;

        while (line[position] != '\0')
        {
            token = getNextTokenFromLine(line, &position);

            if (token.type == TOKEN_ERROR)
            {
                printf("Syntax Error: Unknown token '%s'\n", token.lexeme);
                break;
            }

            if (token.type == TOKEN_KEYWORD)
            {
                if (strcmp(token.lexeme, "read") == 0)
                {
                    // Read from file
                    Token varToken = getNextTokenFromLine(line, &position); // Get variable token
                    if (varToken.type != TOKEN_IDENTIFIER)
                    {
                        printf("Syntax Error: Expected identifier after 'read'.\n");
                        break;
                    }
                    char filename[MAX_BUFFER_SIZE];
                    strcpy(filename, varToken.lexeme);
                    strcat(filename, ".txt"); // Assume text file extension

                    char content[MAX_STRING_SIZE];
                    readStringFromFile(filename, content);

                    printf("%s = \"%s\"\n", varToken.lexeme, content);
                }

                else if (strcmp(token.lexeme, "write") == 0)
                {
                    // Write to file
                    // Token varToken = getNextToken(file); // Get variable token
                    Token varToken = getNextTokenFromLine(line, &position); // Get variable token
                                                                            // printf(varToken.lexeme);
                    if (varToken.type != TOKEN_IDENTIFIER)
                    {
                        printf("Syntax Error: Expected identifier after 'write'.\n");
                        break;
                    }
                    char filename[MAX_BUFFER_SIZE];
                    strcpy(filename, varToken.lexeme);
                    strcat(filename, ".txt"); // Assume text file extension

                    // Token contentToken = getNextToken(file); // Get content token
                    Token contentToken = varToken; // getNextTokenFromLine(line, &position); // Get variable token

                    /*
                    if (contentToken.type != TOKEN_IDENTIFIER )
                    {
                        printf("Syntax Error: Expected identifier or string after 'write'.\n");
                        break;
                    }
                    */
                    if (contentToken.type == TOKEN_IDENTIFIER)
                    {
                        // Resolve identifier to its value
                        // ... perform necessary lookup

                        // For demonstration purposes, assuming variable lookup
                        // returns a string with the identifier as its value
                        char content[MAX_STRING_SIZE];
                        strcpy(content, contentToken.lexeme);
                        printf(contentToken.lexeme);
                        writeStringToFile(filename, content);
                    }
                    else if (contentToken.type == TOKEN_STRING)
                    {
                        printf(contentToken.lexeme);
                        writeStringToFile(filename, contentToken.lexeme);
                    }

                    printf("Successfully wrote to file: %s\n", filename);
                }
                else if (strcmp(token.lexeme, "input") == 0)
                {
                    // Take user input
                    //  Token varToken = getNextToken(file); // Get variable token
                    Token varToken = getNextTokenFromLine(line, &position); // Get variable token
                    if (varToken.type != TOKEN_IDENTIFIER)
                    {
                        printf("Syntax Error: Expected identifier after 'input'.\n");
                        break;
                    }
                    char prompt[MAX_BUFFER_SIZE];
                    strcpy(prompt, varToken.lexeme);

                    char input[MAX_BUFFER_SIZE];
                    getInputFromUser(prompt, input);

                    printf("%s = \"%s\"\n", varToken.lexeme, input);
                }
                else if (strcmp(token.lexeme, "output") == 0)
                {
                    // Output text
                    Token varToken = getNextToken(file); // Get variable token
                    if (varToken.type != TOKEN_IDENTIFIER)
                    {
                        printf("Syntax Error: Expected identifier after 'output'.\n");
                        break;
                    }

                    // Resolve identifier to its value
                    // ... perform necessary lookup

                    // For demonstration purposes, assuming variable lookup
                    // returns a string with the identifier as its value
                    char output[MAX_STRING_SIZE];
                    strcpy(output, varToken.lexeme);

                    outputText(output);
                }
                else
                {
                    printf("Syntax Error: Unknown keyword '%s'\n", token.lexeme);
                    break;
                }
            }
        }
    }
    fclose(file);
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: TextJediInterpreter.exe <filename>\n");
        // Wait for user input before exiting
        getchar();
        return 1;
    }

    const char *filename = argv[1];

    executeTextJediProgram(filename);

    return 0;
}
