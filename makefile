INCLUDES = ./ssl/tls/*.c
FLAGS = -DTEST_DES -DTEST_AES -DTEST_RC4 -DTEST_RSA -DTEST_DIGEST -DTEST_HMAC -DTEST_DSA -DTEST_ECDSA -DTEST_ASN1 -DTEST_X509 -DTEST_PRF -DTEST_PRIVKEY  
OPTS = -g -Wall -Wno-pointer-sign

all:
	g++ -o build/main main.cc ${INCLUDES} -lws2_32 -lm
	g++ http.test.cc -o build/test -lws2_32

run-test:
	./build/test

fest:
	g++ -c main.cc -lws2_32
	g++ -o main.o ./ssl/*.o -lws2_32