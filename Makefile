CCFLAGS= -std=c99 -O3 -lncursesw -lpthread 
OBJOPT= -c

all: trains

trains: main.o
	$(CC) -o trains $(CCFLAGS) main.o 

run: trains
	konsole -e ./trains

main.o: main.c
	$(CC) -o main.o $(OBJOPT) $(CCFLAGS) main.c

clear:
	rm trains *.o
