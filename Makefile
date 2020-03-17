CC = gcc
FLAGS = -Wall -g3
TARGETS = src/list.o src/main.o src/monitor.o src/patientRecord.o src/utilities.o src/hashtable.o

diseaseMonitor:$(TARGETS)
	$(CC) $(FLAGS) -o diseaseMonitor $(TARGETS)

src/list.o:src/list.c
	$(CC) $(FLAGS) -o src/list.o -c src/list.c

src/main.o:src/main.c
	$(CC) $(FLAGS) -o src/main.o -c src/main.c

src/monitor.o:src/monitor.c
	$(CC) $(FLAGS) -o src/monitor.o -c src/monitor.c

src/patientRecord.o:src/patientRecord.c
	$(CC) $(FLAGS) -o src/patientRecord.o -c src/patientRecord.c

src/utilities.o:src/utilities.c
	$(CC) $(FLAGS) -o src/utilities.o -c src/utilities.c

src/hashtable.o:src/hashtable.c
	$(CC) $(FLAGS) -o src/hashtable.o -c src/hashtable.c

.PHONY : clean

clean:
	rm -f $(TARGETS) diseaseMonitor