input = test-encr
output = output-dec

ALL: ENCRYPT DECRYPT
	
DECRYPT: decrypt.c MD5
	gcc -o decrypt decrypt.c

ENCRYPT: encrypt.c MD5
	gcc -o encrypt encrypt.c

MD5: md5.h
	
CLEAN:
	rm -f decrypt
	rm -f encrypt
	make ALL