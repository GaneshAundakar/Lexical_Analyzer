/* main.c - Application entry point with colored ANSI terminal visualization, command-line argument handling, and statistical counters. */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"


int main(int argc, char *argv[]) 
{
    // Verify file parameter target is passed accurately via CLI arguments
    
    if(argc < 2) 
    {
        fprintf(stderr, ANSI_COLOR_RED ANSI_BOLD "Error: No input file specified.\n" ANSI_COLOR_RESET);
      
        fprintf(stderr, "Usage: %s <source_file.c>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Attempt to open the targeted file path
    FILE *src_file = fopen(argv[1], "r");
    
    if (!src_file) 
    {
        fprintf(stderr, ANSI_COLOR_RED ANSI_BOLD "Error: Could not open file '%s'.\n" ANSI_COLOR_RESET, argv[1]);
    
        perror("Details");
        return EXIT_FAILURE;
    }

    // Counters for complete analysis metrics
    int total_tokens = 0;
    int keyword_count = 0;
    int identifier_count = 0;
    int literal_count = 0;
    int operator_count = 0;
    int delimiter_count = 0;
    int unknown_count = 0;

    printf(ANSI_BOLD "\n--- Processing Lexical Tokens For: %s ---\n\n" ANSI_COLOR_RESET, argv[1]);
    printf(ANSI_BOLD "%-10s %-25s %-20s\n" ANSI_COLOR_RESET, "Line No.", "Token Type", "Lexeme Value");
    printf("------------------------------------------------------------\n");

    // Initialize state mapping tracks
    init_lexer(src_file);
    Token t;
    
    // Process input character configurations until terminal block matches
    do 
    {
        t = get_next_token();
    
        if (t.type != TOKEN_EOF) 
        {
            total_tokens++;

            // Select color coding and increment specific tracking values based on type
            switch (t.type) 
            {
                case TOKEN_KEYWORD:
                                    keyword_count++;
                                    printf("%-10d " ANSI_COLOR_CYAN "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                    break;
                    
                case TOKEN_IDENTIFIER:
                case TOKEN_ARRAY_DECL:
                                        identifier_count++;
                                        printf("%-10d " ANSI_COLOR_GREEN "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                        break;
                    
                case TOKEN_LITERAL_INT:
                case TOKEN_LITERAL_FLOAT:
                case TOKEN_LITERAL_CHAR:
                case TOKEN_LITERAL_STRING:
                                            literal_count++;
                                            printf("%-10d " ANSI_COLOR_YELLOW "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                            break;
                    
                case TOKEN_OPERATOR:
                                        operator_count++;
                                        printf("%-10d " ANSI_COLOR_MAGENTA "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                        break;
                    
                case TOKEN_DELIMITER:
                                        delimiter_count++;
                                        printf("%-10d " ANSI_COLOR_WHITE "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                        break;
                    
                case TOKEN_UNKNOWN:
                                    unknown_count++;
                                    printf("%-10d " ANSI_COLOR_RED ANSI_BOLD "%-25s %-20s" ANSI_COLOR_RESET "\n", t.line, token_type_to_string(t.type), t.lexeme);
                                    break;
        
                default:
                    break;
            }
        }
    } while (t.type != TOKEN_EOF);

    // Clean system file locks
    fclose(src_file);

    // Print analysis summary telemetry block with custom formatting accents
    
    printf("------------------------------------------------------------\n");
    printf(ANSI_BOLD "                     ANALYSIS SUMMARY                       \n" ANSI_COLOR_RESET);
    printf("------------------------------------------------------------\n");
    printf(ANSI_COLOR_CYAN "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Keywords Found", keyword_count);
    printf(ANSI_COLOR_GREEN "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Identifiers Found", identifier_count);
    printf(ANSI_COLOR_YELLOW "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Literals Found", literal_count);
    printf(ANSI_COLOR_MAGENTA "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Operators Found", operator_count);
    printf(ANSI_COLOR_WHITE "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Delimiters Found", delimiter_count);
    
    if (unknown_count > 0) 
    {
        printf(ANSI_COLOR_RED ANSI_BOLD "%-30s" ANSI_COLOR_RESET " : %d\n", "Total Errors/Unknown Found", unknown_count);
    }

    printf("------------------------------------------------------------\n");
    printf(ANSI_BOLD "%-30s : %d\n" ANSI_COLOR_RESET, "TOTAL VALID TOKENS ANALYSED", total_tokens);
    printf("------------------------------------------------------------\n\n");

    return EXIT_SUCCESS;
}

