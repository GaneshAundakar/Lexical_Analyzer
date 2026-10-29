/* lexer.h
 * Token classifications and core definitions for the Lexical Analyzer.
 * This file contains definitions for token types, structural metadata for tokens and declarations for tracking source files and extraction loops.
 */

#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdbool.h>

typedef enum 
{
    TOKEN_KEYWORD,          // Built-in C syntax keywords (e.g., int, float, return)
    TOKEN_IDENTIFIER,       // Variable names, function names, and label definitions
    TOKEN_LITERAL_INT,      // Integer numeric constants (e.g., 95)
    TOKEN_LITERAL_FLOAT,    // Fractional / Floating point numerical constants (e.g., 3.1415)
    TOKEN_LITERAL_CHAR,     // Character literals wrapped in single quotes (e.g., 'A')
    TOKEN_LITERAL_STRING,   // String constants wrapped in double quotes (e.g., "success")
    TOKEN_ARRAY_DECL,       // Array explicit designations (e.g., arr[10] or flag[])
    TOKEN_OPERATOR,         // Arithmetic, logical, and evaluation symbols (e.g., +, >=)
    TOKEN_DELIMITER,        // Block markers, punctuation, and line terminals (e.g., {, }, ;)
    TOKEN_UNKNOWN,          // Fallback tracking identifier for unrecognized/illegal symbols
    TOKEN_EOF               // Terminal condition indicating structural end-of-file reached
} TokenType;

/* struct Token - Bundles the classification type, physical string value, and position context. */

typedef struct 
{
    TokenType type;         // Evaluated classification group
    char lexeme[256];       // Extracted raw string value captured from source code
    int line;               // Source code line sequence marker for logging and tracing
} Token;


// Define ANSI Escape Codes for formatting terminal text output
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_CYAN    "\x1b[36m"   // Keywords
#define ANSI_COLOR_GREEN   "\x1b[32m"   // Identifiers & Arrays
#define ANSI_COLOR_YELLOW  "\x1b[33m"   // Literals (Int, Float, Char, String)
#define ANSI_COLOR_MAGENTA "\x1b[35m"   // Operators
#define ANSI_COLOR_WHITE   "\x1b[37m"   // Delimiters
#define ANSI_COLOR_RED     "\x1b[31m"   // Errors / Unknown tokens
#define ANSI_BOLD          "\x1b[1m"


void init_lexer(FILE *source_file); // source_file is Pointer to an open read-ready file stream.

Token get_next_token();

/*Converts enum classifications into predictable human-readable string labels. */

const char* token_type_to_string(TokenType type); //  type - The TokenType configuration evaluated by the scanning phase.

#endif // LEXER_H
