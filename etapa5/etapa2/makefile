IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR = .
LDIR = .

LIBS = -lm

_DEPS = tokens.h parser.tab.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lex.yy.o parser.tab.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: etapa2

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# --- Special rule: generate lex.yy.c from scanner.l ---
$(ODIR)/lex.yy.c : scanner.l
	flex -o $@ $<

# --- Special rule: generate parser.tab.c and parser.tab.h from parser.y ---
$(ODIR)/parser.tab.c $(ODIR)/parser.tab.h: parser.y
	bison -d --report=all --report-file=$(ODIR)/parser.output -o $(ODIR)/parser.tab.c $<

etapa2: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Create test directories
.PHONY: create-test-dirs
create-test-dirs:
	@mkdir -p tests/valid
	@mkdir -p tests/invalid

# Run tests
.PHONY: test

test: create-test-dirs
	bash test_syntax.sh

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(ODIR)/parser.tab.c $(ODIR)/parser.tab.h