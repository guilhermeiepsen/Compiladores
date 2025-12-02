# Compiler
CC = gcc

# Bison and Flex
BISON = bison
FLEX = flex

# Flags
# -g para debug, -fsanitize para vazamentos de memória (requerido na E4/E5)
CFLAGS = -Wall -g -fsanitize=address
# Adicionado -I. para incluir headers na raiz (como iloc.h)
CFLAGS += -I. -Iparser -Iasd -Iscope

# Files
LEX_FILE = scanner.l
YACC_FILE = parser/parser.y
LEX_C_FILE = parser/lex.yy.c
YACC_C_FILE = parser/parser.tab.c
YACC_H_FILE = parser/parser.tab.h

# Output
EXEC = etapa5

# Rules
all: $(EXEC)

# Adicionado iloc.o na lista de dependências
$(EXEC): $(YACC_C_FILE) $(LEX_C_FILE) main.o asd.o scope.o iloc.o
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c
	$(CC) $(CFLAGS) -c $<

asd.o: asd/asd.c
	$(CC) $(CFLAGS) -c $<

scope.o: scope/scope.c
	$(CC) $(CFLAGS) -c $<

iloc.o: iloc.c iloc.h
	$(CC) $(CFLAGS) -c $<

$(YACC_C_FILE) $(YACC_H_FILE): $(YACC_FILE)
	$(BISON) -d -v -o $(YACC_C_FILE) $<

$(LEX_C_FILE): $(LEX_FILE)
	$(FLEX) -o $@ $<

# Clean melhorado
clean:
	rm -f $(EXEC) *.o 
	rm -f $(LEX_C_FILE) 
	rm -f $(YACC_C_FILE) $(YACC_H_FILE) 
	rm -f parser.output parser/parser.output 
	rm -f lex.yy.c parser.tab.c parser.tab.h

.PHONY: all clean