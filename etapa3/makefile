# Compiler
CC = gcc

# Bison and Flex
BISON = bison
FLEX = flex

# Flags
CFLAGS = -Wall -g

# Files
LEX_FILE = scanner.l
YACC_FILE = parser.y
LEX_C_FILE = lex.yy.c
YACC_C_FILE = parser.tab.c
YACC_H_FILE = parser.tab.h

# Output
EXEC = ast_parser

# Rules
all: $(EXEC)

$(EXEC): $(YACC_C_FILE) $(LEX_C_FILE) main.o asd.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

asd.o: asd.c
	$(CC) $(CFLAGS) -c $<

$(YACC_C_FILE) $(YACC_H_FILE): $(YACC_FILE)
	$(BISON) -d $<

$(LEX_C_FILE): $(LEX_FILE)
	$(FLEX) $<

clean:
	rm -f $(EXEC) *.o $(LEX_C_FILE) $(YACC_C_FILE) $(YACC_H_FILE)

.PHONY: all clean
