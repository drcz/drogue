CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-lncurses
SOURCES= akcje.c decydent.c gen.c komunikaty.c losuj.c main.c obrazki.c pociski.c pojemnik.c przedmioty.c rysuj.c smierc.c stany.c stwory.c teren.c widz.c wejscie.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=drogue

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@