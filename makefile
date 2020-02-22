IDIR=./include
CC=g++
OPENSSL=/src/include/openssl

CFLAGS=-I$(IDIR) -I$(OPENSSL) -lcrypto

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = \
	common.h \
	parser.h \
	cry.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = \
	main.o \
	parser.o \
	cry.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

CryPass: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
