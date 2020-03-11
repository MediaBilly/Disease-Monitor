CC = gcc
FLAGS = -Wall
SOURCES = $(wildcard ./src/*.c)
OBJECTS = $(SOURCES:%.c=%.o)

all:diseaseMonitor

diseaseMonitor:$(OBJECTS)
	$(CC) $(FLAGS) -o diseaseMonitor $(OBJECTS)

%.o:./src/%.c
	$(CC) $(FLAGS) -o $@ -c $<

.PHONY : clean

clean:
	rm -f $(OBJECTS) diseaseMonitor