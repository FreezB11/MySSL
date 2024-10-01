INCLUDES = ./tls-0.1/*.o
OPTS = -g -Wall -Wno-pointer-sign

all:
	g++ -c main.cc ./tls-0.1/*.cc
	gcc -c ./tls-0.1/*.c
	mv *.o ./obj
	g++ -o build/main ./obj/*.o -lm

cc:
	gcc -c main main.c ./tls-0.1/*.c -lm
	mv *.o ./obj
	gcc -o build/cc ./obj/*.o -lm

run:
	./build/main

clean:
	cd obj && rm *.o