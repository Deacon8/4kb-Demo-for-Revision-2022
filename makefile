IDIR =
CC=gcc
CFLAGS=-I$(IDIR) -g -Wall
OUTPUT=bin/demo

SDIR=
#Should be ./obj but it works
ODIR=

LIBS= -lopengl32 -lgdi32 -lmingw32 -L./libs -lglfw3

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	del .\src\*.o
