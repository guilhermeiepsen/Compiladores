IDIR =.
CC=gcc
CFLAGS=-I$(IDIR)

ODIR = .
LDIR = .

LIBS = -lm

_DEPS = tokens.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lex.yy.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: etapa1


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

# --- Special rule: generate lex.yy.c from scanner.l ---
$(ODIR)/lex.yy.c : scanner.l
	flex -o $@ $<

etapa1: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 