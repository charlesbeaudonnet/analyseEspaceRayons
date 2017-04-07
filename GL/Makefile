CC=g++
OPT=-lglut -lGL -std=c++11

all: main.o viewer.o point.o
	$(CC) main.o viewer.o point.o $(OPT) -o testGL
point.o:
	$(CC) -c point.cpp -o point.o
viewer.o:
	$(CC) -c viewer.cpp -o viewer.o 
main.o:
	$(CC) -c helloworld.cpp -o main.o
clean:
	rm -f *.o