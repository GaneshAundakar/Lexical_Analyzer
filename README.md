# C Lexical Analyzer

A high-performance Lexical Analyzer (Lexer) written in C that scans C source code files and breaks them down into individual lexical tokens. It prints a detailed table of line numbers, token classifications, and lexeme values in your terminal, complete with bright, color-coded ANSI formatting and a final statistics summary sheet.

## Features

- Robust Token Scanning: Parses standard identifiers, C keywords, string/character literals, operations, and nested delimiters.
- Advanced Code Sanitization: Safely handles and strips out both single-line (//) and multi-line (/* ... */) code comments while preserving proper track of newline spacing.
- Array Recognition Modifiers: Tracks code array formatting logic instantly to differentiate base variables from explicit structural definitions (e.g., matching arr[index]).
- Color-Coded CLI Terminal Engine: Utilizes distinct ANSI typography profiles to label lexical matches directly in runtime columns (Green for Identifiers, Cyan for Keywords, Yellow for Literals, Red for Errors, etc.).
- Summary Telemetry Diagnostics: Evaluates the comprehensive project score and renders a final summary layout breaking down operational frequencies.

---

## Token Classifications Matrix

- TOKEN_KEYWORD (KEYWORD): Reserved words like int, return, if, while, static, etc.
- TOKEN_IDENTIFIER (IDENTIFIER): Standard variables, methods, and functions (main, src_file).
- TOKEN_LITERAL_INT (DECIMAL_LITERAL): Whole numerical digits (e.g., 1, 4000, 255).
- TOKEN_LITERAL_FLOAT (FLOAT_LITERAL): Floating-point point metrics (e.g., 3.14).
- TOKEN_LITERAL_STRING (STRING_LITERAL): Sequences grouped safely in quotation brackets ("Error: No input file.").
- TOKEN_ARRAY_DECL (ARRAY_IDENTIFIER): Arrays tracked using explicit notation parameters ([size]).
- TOKEN_OPERATOR (OPERATOR): Math, comparison, and logic symbols (+, -, *, /, ==, &&).
- TOKEN_DELIMITER (DELIMITER): Functional structural markers ( (, ), {, }, ;, , ).
- TOKEN_UNKNOWN (UNKNOWN_ERROR): Characters unrecognized by the modern lexer rule configuration.

---

## Project Structure

- main.c : Application entry point, CLI arguments, and ANSI telemetry output
- lexer.c : Core stream tokenization loops, identifier checks, and parser rules
- lexer.h : Token data models, type definitions, and ANSI color maps
- Makefile : Automates compiling, building, and workspace cleanups
- README.md : Documentation and usage guide

---

## Getting Started

### Prerequisites
Make sure you have make and a standard C compiler toolchain installed on your workstation (such as gcc or clang).

### 1. Compilation
Build the executable automatically using your workspace Makefile configuration:
make

### 2. Running the Lexer
Provide the path of the specific target source file you wish to process as a command-line argument to the compiled binary:
./LexicalAnalyzer.exe sample_code.c

### 3. Cleaning Build Objects
To clear intermediate .o object files and the raw binaries from your repository directory, run:
make clean

---

## Example Runtime Output

--- Processing Lexical Tokens For: sample_code.c ---

Line No.   Token Type                Lexeme Value        
------------------------------------------------------------
1          KEYWORD                   int                 
1          IDENTIFIER                counter             
1          OPERATOR                  =                   
1          DECIMAL_LITERAL           0                   
1          DELIMITER                 ;                   
2          KEYWORD                   if                  
2          DELIMITER                 (                   
2          IDENTIFIER                counter             
2          OPERATOR                  ==                  

------------------------------------------------------------
                     ANALYSIS SUMMARY                       
------------------------------------------------------------
Total Keywords Found           : 12
Total Identifiers Found        : 24
Total Literals Found           : 8
Total Operators Found          : 5
Total Delimiters Found         : 18
------------------------------------------------------------
TOTAL VALID TOKENS ANALYSED    : 67
------------------------------------------------------------
