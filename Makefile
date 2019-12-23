CC=gcc
CFLAGS=-c -I/usr/local/pgsql/include -I/usr/include/libxml2
LDFLAGS=
DEPS=-lpq -lxml2 -lm
SOURCES=main.c bdd.c sun.c ftrend.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=pred_ftrend

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(DEPS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@ $(DEPS)

clean:
	rm -f *.o test
