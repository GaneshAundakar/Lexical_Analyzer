# Define the compiler to use
CC = gcc

# Define compilation flags (-Wall shows all warnings, -g adds debugging info)
CFLAGS = -Wall -g

# Define the final executable binary name
TARGET = LexicalAnalyzer.exe 

# Define the object files needed to link the program
OBJ = main.o lexer.o

# The default rule that runs when you type 'make'
LexicalAnalyzer : $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile main.c into main.o (depends on lexer.h tracking)
main.o: main.c lexer.h
	$(CC) $(CFLAGS) -c main.c

# Rule to compile lexer.c into lexer.o (depends on lexer.h tracking)
lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

# Rule to clean up intermediate object files and binary outputs
clean:
	rm -f $(OBJ) $(TARGET)
