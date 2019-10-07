input = test-encr
output = output-dec

ALL: ENCRYPT DECRYPT BRUTE
	
DECRYPT: decrypt.c MD5
	gcc -m32 -o decrypt decrypt.c

ENCRYPT: encrypt.c MD5
	gcc -m32 -o encrypt encrypt.c
	
BRUTE: brute-force.c MD5
	gcc -m32 -o brute-force brute-force.c

MD5: md5.h
	
CLEAN:
	rm -f decrypt
	rm -f encrypt
	rm -f brute-force
	make ALL