/* lexer.c
 * Implements character-stream evaluation loops and conditional parsing patterns. */

#include "lexer.h"
#include <string.h>
#include <ctype.h>

// Static contextual tracking properties locked to this compilation unit
static FILE *src = NULL;
static int current_line = 1;

// Standard C Keywords targeted for confirmation mapping

static const char *keywords[] = { "int", "float", "char", "double", "return", "if", "else", 
    "while", "for", "do", "void", "switch", "case", "break", 
    "continue", "struct", "typedef", "static", "const" };

static const int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

void init_lexer(FILE *source_file) 
{
    src = source_file;
    current_line = 1; // Reset operational trackers upon bind
}

/* Tests whether an extracted identifier string matches a standard reserved word. */

static bool is_keyword(const char *str) 
{
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(str, keywords[i]) == 0) return true;
    }
    return false;
}

Token get_next_token() 
{
    Token token;
    token.type = TOKEN_UNKNOWN;
    token.line = current_line;
    memset(token.lexeme, 0, sizeof(token.lexeme));
    
    int idx = 0;
    int ch;

    while ((ch = fgetc(src)) != EOF) 
    {
        // Track newlines to preserve precise positioning data
        if (ch == '\n') 
        {
            current_line++;
            token.line = current_line;
            continue;
        }
        
        // Skip tabs, spaces, and formatting characters safely
        if (isspace(ch)) continue;

        // Skip comments (both single-line '//' and block style '/* */')
        if (ch == '/') 
        {
            int next = fgetc(src);
        
            if(next == '/') 
            {
                // Consume line until boundary or structural EOF is met
              
                while ((ch = fgetc(src)) != '\n' && ch != EOF);
                
                if (ch == '\n') current_line++;
                token.line = current_line;
                continue;
            }
            else if (next == '*') 
            {
                // Consume entire multi-line block safely tracking inside iterations
                while (true) 
                {
                    ch = fgetc(src);
                
                    if (ch == EOF) break;
                
                    if (ch == '\n') current_line++;
                
                    if (ch == '*')
                    {
                        if ((ch = fgetc(src)) == '/') break;
                        ungetc(ch, src);
                    }
                }
                token.line = current_line;
                continue;
            } 
            else
            {
                ungetc(next, src); // Revert read state if single '/' division operator
            }
        }

        // Process String Literals
        if (ch == '"') 
        {
            token.type = TOKEN_LITERAL_STRING;
        
            while ((ch = fgetc(src)) != '"' && ch != EOF) 
            {
                if (ch == '\n') current_line++;
                if (idx < 255) token.lexeme[idx++] = ch;
            }
            
            token.lexeme[idx] = '\0';
            return token;
        }

        // Process Character Literals
        if (ch == '\'') 
        {
            token.type = TOKEN_LITERAL_CHAR;
        
            while ((ch = fgetc(src)) != '\'' && ch != EOF) 
            {
                if (ch == '\n') current_line++;
                if (idx < 255) token.lexeme[idx++] = ch;
            }
            token.lexeme[idx] = '\0';
            return token;
        }

        // Process Numeric Literals (Decimals & Floats)
        if (isdigit(ch)) 
        {
            token.lexeme[idx++] = ch;
            bool is_float = false;
        
            while ((ch = fgetc(src)) != EOF) 
            {
                if (isdigit(ch)) 
                {
                    if (idx < 255) token.lexeme[idx++] = ch;
                } 
                else if (ch == '.' && !is_float) 
                {
                    is_float = true; // Flag floating point context
                    if (idx < 255) token.lexeme[idx++] = ch;
                } 
                else
                {
                    ungetc(ch, src); // Return non-digit element back to lookahead stream
                    break;
                }
            }

            token.lexeme[idx] = '\0';
            
            token.type = is_float ? TOKEN_LITERAL_FLOAT : TOKEN_LITERAL_INT;
            
            return token;
        }

        // Process Identifiers, Keywords, and Array Declarations
        if (isalpha(ch) || ch == '_') 
        {
            token.lexeme[idx++] = ch;
        
            while ((ch = fgetc(src)) != EOF && (isalnum(ch) || ch == '_')) 
            {
                if (idx < 255) token.lexeme[idx++] = ch;
            }
            token.lexeme[idx] = '\0';

            // Check if trailing structures indicate an Array Designation
            int lookahead = ch;
            while (lookahead != EOF && isspace(lookahead)) 
            {
                lookahead = fgetc(src);
            }
            
            if (lookahead == '[') 
            {
                token.type = TOKEN_ARRAY_DECL;
                token.lexeme[idx++] = lookahead;
            
                while ((ch = fgetc(src)) != EOF && ch != ']') 
                {
                    if (idx < 255) token.lexeme[idx++] = ch;
                }
                
                if (ch == ']') token.lexeme[idx++] = ']';
                
                token.lexeme[idx] = '\0';
                
                return token;
            } 
            else
            {
                if (lookahead != EOF) ungetc(lookahead, src);
            }

            // Distinguish standard identifier names from core code keywords
            if (is_keyword(token.lexeme)) 
            {
                token.type = TOKEN_KEYWORD;
            }
            else 
            {
                token.type = TOKEN_IDENTIFIER;
            }
            return token;
        }

        // Process Composite or Single Operators
        if(strchr("+-*/%=<>!&|^~", ch)) 
        {
            token.type = TOKEN_OPERATOR;
            token.lexeme[idx++] = ch;
            int next = fgetc(src);
          
            // Handle double-symbol configurations like +=, ==, <=, &&
            if (strchr("=+-&|<>", next)) 
            {
                token.lexeme[idx++] = next;
            }
            else
            {
                ungetc(next, src);
            }
            
            token.lexeme[idx] = '\0';
            
            return token;
        }

        // Process Delimiters / Block Punctuations
        if (strchr("(){}[];,.:", ch)) 
        {
            token.type = TOKEN_DELIMITER;
            token.lexeme[0] = ch;
            token.lexeme[1] = '\0';
            return token;
        }

        // Handle Unknown Symbols
        token.type = TOKEN_UNKNOWN;
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
        return token;
    }

    // Set EOF termination states
    token.type = TOKEN_EOF;
    strcpy(token.lexeme, "EOF");
    return token;
}

const char* token_type_to_string(TokenType type) 
{
    switch (type) 
    {
        case TOKEN_KEYWORD:        return "KEYWORD";
        case TOKEN_IDENTIFIER:     return "IDENTIFIER";
        case TOKEN_LITERAL_INT:    return "DECIMAL_LITERAL";
        case TOKEN_LITERAL_FLOAT:  return "FLOAT_LITERAL";
        case TOKEN_LITERAL_CHAR:   return "CHAR_LITERAL";
        case TOKEN_LITERAL_STRING: return "STRING_LITERAL";
        case TOKEN_ARRAY_DECL:     return "ARRAY_IDENTIFIER";
        case TOKEN_OPERATOR:       return "OPERATOR";
        case TOKEN_DELIMITER:      return "DELIMITER";
        default:                   return "UNKNOWN_ERROR";
    }
}

/* End of lexer.c file */