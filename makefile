INCLUDES = ./tls-0.1/*.o
OPTS = -g -Wall -Wno-pointer-sign

all:
	g++ -c main.cc ./tls-0.1/*.cc
	gcc -c ./tls-0.1/*.c
	g++ -o build/main *.o -lws2_32 -lm


run-test:
	./build/test