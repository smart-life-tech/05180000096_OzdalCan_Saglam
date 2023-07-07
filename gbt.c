#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Token types
typedef enum {
    INTEGER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    EOF_TOKEN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    int value;
} Token;

// Lexer
typedef struct {
    const char* text;
    int pos;
    char current_char;
} Lexer;

void lexer_error() {
    fprintf(stderr, "Invalid character\n");
    exit(1);
}

void lexer_init(Lexer* lexer, const char* text) {
    lexer->text = text;
    lexer->pos = 0;
    lexer->current_char = lexer->text[lexer->pos];
}

void lexer_advance(Lexer* lexer) {
    lexer->pos++;
    if (lexer->text[lexer->pos] != '\0') {
        lexer->current_char = lexer->text[lexer->pos];
    } else {
        lexer->current_char = '\0';
    }
}

void lexer_skip_whitespace(Lexer* lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        lexer_advance(lexer);
    }
}

int lexer_integer(Lexer* lexer) {
    char result[100];
    int i = 0;

    while (lexer->current_char != '\0' && isdigit(lexer->current_char)) {
        result[i++] = lexer->current_char;
        lexer_advance(lexer);
    }

    result[i] = '\0';
    return atoi(result);
}

Token lexer_get_next_token(Lexer* lexer) {
    while (lexer->current_char != '\0') {
        if (isspace(lexer->current_char)) {
            lexer_skip_whitespace(lexer);
            continue;
        }

        if (isdigit(lexer->current_char)) {
            Token token;
            token.type = INTEGER;
            token.value = lexer_integer(lexer);
            return token;
        }

        if (lexer->current_char == '+') {
            lexer_advance(lexer);
            Token token;
            token.type = PLUS;
            return token;
        }

        if (lexer->current_char == '-') {
            lexer_advance(lexer);
            Token token;
            token.type = MINUS;
            return token;
        }

        if (lexer->current_char == '*') {
            lexer_advance(lexer);
            Token token;
            token.type = MULTIPLY;
            return token;
        }

        if (lexer->current_char == '/') {
            lexer_advance(lexer);
            Token token;
            token.type = DIVIDE;
            return token;
        }

        lexer_error();
    }

    Token token;
    token.type = EOF_TOKEN;
    return token;
}

// Parser
typedef struct {
    Lexer lexer;
    Token current_token;
} Parser;

void parser_init(Parser* parser, const char* text) {
    lexer_init(&(parser->lexer), text);
    parser->current_token = lexer_get_next_token(&(parser->lexer));
}

void parser_error() {
    fprintf(stderr, "Invalid syntax\n");
    exit(1);
}

void parser_eat(Parser* parser, TokenType token_type) {
    if (parser->current_token.type == token_type) {
        parser->current_token = lexer_get_next_token(&(parser->lexer));
    } else {
        parser_error();
    }
}

int parser_factor(Parser* parser) {
    Token token = parser->current_token;
    parser_eat(parser, INTEGER);
    return token.value;
}

int parser_term(Parser* parser) {
    int result = parser_factor(parser);

    while (parser->current_token.type == MULTIPLY || parser->current_token.type == DIVIDE) {
        Token token = parser->current_token;
        if (token.type == MULTIPLY) {
            parser_eat(parser, MULTIPLY);
            result *= parser_factor(parser);
        } else if (token.type == DIVIDE) {
            parser_eat(parser, DIVIDE);
            result /= parser_factor(parser);
        }
    }

    return result;
}

int parser_expr(Parser* parser) {
    int result = parser_term(parser);

    while (parser->current_token.type == PLUS || parser->current_token.type == MINUS) {
        Token token = parser->current_token;
        if (token.type == PLUS) {
            parser_eat(parser, PLUS);
            result += parser_term(parser);
        } else if (token.type == MINUS) {
            parser_eat(parser, MINUS);
            result -= parser_term(parser);
        }
    }

    return result;
}

int main() {
    char expression[100];
    printf("Enter an arithmetic expression: ");
    fgets(expression, sizeof(expression), stdin);

    Parser parser;
    parser_init(&parser, expression);
    int result = parser_expr(&parser);

    printf("Result: %d\n", result);
while (1)
{
    /* code */
    
}

    return 0;
}
