SOURCES = $(wildcard *.c)

OBJECTS = $(SOURCES:.c=.o)
LDLIBS = -lm
FLAGS = -Wall -Wextra -pedantic
CFLAGS = $(FLAGS) -s -O3 -DNDEBUG

all: cx_test

dbg: CFLAGS = $(FLAGS) -g
dbg: cx_test

cx_test: $(OBJECTS)

clean:
	rm -f core $(OBJECTS) cx_test
