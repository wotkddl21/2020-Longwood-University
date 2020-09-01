CC=gcc
CFLAGS=-g -c -I/usr/local/include
LDFLAGS=-L/usr/local/lib
SOURCES=main.c flib.c
LIBS=
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=feeble

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LIBS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(EXECUTABLE)
	touch *
