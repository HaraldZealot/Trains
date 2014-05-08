CXXFLAGS= -std=c++11 -O3 -lncursesw -lpthread 
OBJOPT= -c

all: trains

trains: main.o
	$(CXX) -o trains $(CXXFLAGS) main.o 

run: trains
	konsole -e ./trains

main.o: main.cpp
	$(CXX) -o main.o $(OBJOPT) $(CXXFLAGS) main.cpp

clean:
	rm trains *.o
