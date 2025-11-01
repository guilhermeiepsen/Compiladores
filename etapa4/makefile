# Compiler
CC = gcc

# Bison and Flex
BISON = bison
FLEX = flex

# Flags
CFLAGS = -Wall -g -fsanitize=address
CFLAGS += -Iparser -Iasd -Iscope

# Files
LEX_FILE = scanner.l
YACC_FILE = parser/parser.y
LEX_C_FILE = parser/lex.yy.c
YACC_C_FILE = parser/parser.tab.c
YACC_H_FILE = parser/parser.tab.h

# Output
EXEC = etapa4

# Rules
all: $(EXEC)

$(EXEC): $(YACC_C_FILE) $(LEX_C_FILE) main.o asd.o scope.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

asd.o: asd/asd.c
	$(CC) $(CFLAGS) -c $<

scope.o: scope/scope.c
	$(CC) $(CFLAGS) -c $<

$(YACC_C_FILE) $(YACC_H_FILE): $(YACC_FILE)
	$(BISON) -d -v -o $(YACC_C_FILE) $<

$(LEX_C_FILE): $(LEX_FILE)
	$(FLEX) -o $@ $<

clean:
	rm -f $(EXEC) *.o $(LEX_C_FILE) $(YACC_C_FILE) $(YACC_H_FILE) parser.output parser/parser.output lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean
