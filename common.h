#pragma once

static char *keywords[] = {
    "new",
    "int",
    "text",
    "size",
    "subs",
    "locate",
    "insert",
    "override",
    "read",
    "write",
    "from",
    "to",
    "input",
    "output",
    "asText",
    "asString",
};
static int keyword_count = sizeof(keywords) / sizeof(char *);

static char errbuf[255] = {'\0'};

char *add_char(char *orig, char c);
void fatal(char *msg, int line, int pos);
int check_is_keyword(char *str);
