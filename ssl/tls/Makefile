all: des aes rc4 rsa digest hmac dsa ecdsa asn1 x509 prf privkey

OPTS = -g -Wall -Wno-pointer-sign

des: des.c des.h hex.c hex.h
	gcc -DTEST_DES -o des $(OPTS) des.c hex.c

aes: aes.c aes.h hex.c hex.h
	gcc -DTEST_AES -o aes $(OPTS) aes.c hex.c

rc4: rc4.c rc4.h hex.c hex.h
	gcc -DTEST_RC4 -o rc4 $(OPTS) rc4.c hex.c

rsa: rsa.c rsa.h huge.c huge.h hex.c hex.h 
	gcc  -DTEST_RSA -o rsa $(OPTS) rsa.c huge.c hex.c

digest: digest.c digest.h md5.c md5.h sha.c sha.h ../ch02/hex.c ../ch02/hex.h
	gcc  -DTEST_DIGEST $(OPTS) -o digest digest.c md5.c sha.c ../ch02/hex.c -lm

hmac: hmac.c hmac.h digest.c digest.h md5.c md5.h sha.c sha.c ../ch02/hex.c ../ch02/hex.h
	gcc  -DTEST_HMAC $(OPTS) -o hmac hmac.c digest.c md5.c sha.c ../ch02/hex.c -lm

dsa: dsa.c dsa.h sha.c sha.h digest.c digest.h ../ch03/huge.c ../ch03/huge.h ../ch02/hex.c ../ch02/hex.h
	gcc -I../ch03  -DTEST_DSA $(OPTS) -o dsa dsa.c digest.c sha.c ../ch03/huge.c ../ch02/hex.c

ecdsa: ecdsa.c ecdsa.h ecc.c ecc.h dsa.h sha.c sha.h digest.c digest.h ../ch03/huge.h ../ch03/huge.c ../ch02/hex.c ../ch02/hex.h
	gcc  -I../ch03 -DTEST_ECDSA $(OPTS) -o ecdsa ecdsa.c ecc.c digest.c sha.c ../ch03/huge.c ../ch02/hex.c

asn1: asn1.c asn1.h ../ch01/base64.c ../ch01/base64.h
	gcc -I../ch01 -DTEST_ASN1 $(OPTS) -o asn1 asn1.c ../ch01/base64.c

x509: x509.c x509.h asn1.c asn1.h ../ch01/base64.c ../ch01/base64.h ../ch02/hex.c ../ch02/hex.h ../ch03/huge.c ../ch03/huge.h ../ch03/rsa.c ../ch03/rsa.h ../ch04/dsa.c ../ch04/dsa.h ../ch04/digest.c ../ch04/digest.h ../ch04/md5.c ../ch04/md5.h ../ch04/sha.c ../ch04/sha.h
	gcc -I../ch01  -I../ch03 -I../ch04 -DTEST_X509 $(OPTS) -o x509 x509.c asn1.c ../ch01/base64.c ../ch02/hex.c ../ch03/huge.c ../ch03/rsa.c ../ch04/dsa.c ../ch04/digest.c ../ch04/sha.c ../ch04/md5.c -lm

prf: prf.c prf.h ../ch04/hmac.c ../ch04/hmac.h ../ch04/md5.c ../ch04/md5.h ../ch04/sha.c ../ch04/sha.h ../ch04/digest.c ../ch04/digest.h ../ch02/hex.c ../ch02/hex.h
	gcc -I../ch04  -DTEST_PRF $(INCLUDES) $(OPTS) -o prf prf.c ../ch04/hmac.c ../ch04/md5.c ../ch04/sha.c ../ch04/digest.c ../ch02/hex.c ../ch02/hex.h -lm

privkey: privkey.c privkey.h file.c file.h ../ch01/base64.c ../ch01/base64.h ../ch02/hex.c ../ch02/hex.h ../ch02/des.c ../ch02/des.h ../ch03/huge.c ../ch03/huge.h ../ch03/rsa.c ../ch03/rsa.h ../ch04/digest.c ../ch04/digest.h ../ch04/md5.c ../ch04/md5.h ../ch04/sha.c ../ch04/sha.c ../ch05/asn1.c ../ch05/asn1.h
	gcc -I../ch01  -I../ch03 -I../ch05 -DTEST_PRIVKEY $(INCLUDES) $(OPTS) -o privkey privkey.c file.c ../ch01/base64.c ../ch02/hex.c ../ch02/des.c ../ch03/huge.c ../ch03/rsa.c ../ch04/digest.c ../ch04/md5.c ../ch04/sha.c ../ch05/asn1.c -lm


clean:
	rm -f des
	rm -f aes
	rm -f rc4
	rm -f core
	rm -f core.*
	rm -f rsa
	rm -f core
	rm -f core.*
	rm -f md5
	rm -f hmac
	rm -f digest
	rm -f dsa
	rm -f ecdsa
	rm -f asn1
	rm -f x509
	rm -f prf
	rm -f privkey
