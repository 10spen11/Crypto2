#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "md5.h"

// input : ./prog key

unsigned int key;
int buf, n, infile, outfile;
MD5_CTX mdContext; 


lastbytes(int outfile, int size, int buf) // magic code for extracting last bytes of encryption without the padding
{ 
  int i = 0;
  char *last;
  last = (char*) &buf;
  for (i=0;i<size;i++) {write(outfile, &last[i], 1);} 
};

int bruteForce()
{
  struct stat st;
  int size,fsize;
  int *temp, result;   
  int rollingkey;
  int buffer[250000];
  unsigned char charBuf1, charBuf2, charBuf3, charBuf4;
  int i = 0;
  int length;
  rollingkey = 0;   
 
  infile = open ("output", O_RDONLY);
  if (infile<0) { printf("input open error\n"); exit(0); }
  
  buf = 0;
  read(infile,&buf,4);
  size=buf; // get plaintext size

  // ciphertext has xtra 4 bytes (size) and padding 

  stat("output", &st); fsize = st.st_size; // get ciphertext size
  if ((fsize < 8)||(size>fsize)||(size<(fsize-8))) {printf("file size sanity check failed\n");}; 
  
  printf("file size: %i\n", size);


  while ((n = read(infile, &buf, 4)) > 0)
  {
	  buffer[i] = buf;
	  ++i;

  }
  length = i;

  close(infile);
  printf("Read all the Data\n");

  unsigned int maxInt = 0;
  maxInt = (unsigned int) -1;

  int key = 0; 
  int max = 0;
  int goodKeyCount = 0;


  for (unsigned int j = 0; j < maxInt; ++j)
  {

	  int likelyhood = 0;
	  rollingkey = j;

	  buf = buffer[0] ^ rollingkey; // doing the reverse of encrypt
		  /*
		  MD5Init(&mdContext);
		  MD5Update(&mdContext, &rollingkey, 4);
		  MD5Final(&mdContext);
		  temp = (int*)& mdContext.digest[12];
		  result = *temp; // result is 32 bits of MD5 of key
		  rollingkey = rollingkey ^ result; // new key
		  */

		  // we don't care about the last 4 bytes, we're brute forcing the key, not the message

	  charBuf1 = buf & 255;
	  buf = buf >> 8;
	  charBuf2 = buf & 255;
	  buf = buf >> 8;
	  charBuf3 = buf & 255;
	  buf = buf >> 8;
	  charBuf4 = buf & 255;


	  //weigh likelyhoods based on arbitrary metrics
	  if (charBuf1 == ' ')
		  likelyhood += 1;
	  if (charBuf2 == ' ')
		  likelyhood += 1;
	  if (charBuf3 == ' ')
		  likelyhood += 1;
	  if (charBuf4 == ' ')
		  likelyhood += 1;

	  if ((charBuf1 >= 'A' && charBuf1 <= 'Z') || (charBuf1 >= 'a' && charBuf1 <= 'z'))
		  likelyhood += 1;
	  if ((charBuf2 >= 'A' && charBuf2 <= 'Z') || (charBuf2 >= 'a' && charBuf2 <= 'z'))
		  likelyhood += 1;
	  if ((charBuf3 >= 'A' && charBuf3 <= 'Z') || (charBuf3 >= 'a' && charBuf3 <= 'z'))
		  likelyhood += 1;
	  if ((charBuf4 >= 'A' && charBuf4 <= 'Z') || (charBuf4 >= 'a' && charBuf4 <= 'z'))
		  likelyhood += 1;

	  // tests if this is a good key
	  if (likelyhood >= 4)
	  {
		  goodKeyCount++;

		  // do further tests
		  for (unsigned int i = 1; i < length; ++i)
		  {
			  MD5Init(&mdContext);
			  MD5Update(&mdContext, &rollingkey, 4);
			  MD5Final(&mdContext);
			  temp = (int*)& mdContext.digest[12];
			  result = *temp; // result is 32 bits of MD5 of key
			  rollingkey = rollingkey ^ result; // new key
			  buf = buffer[i] ^ rollingkey; // doing the reverse of encrypt

			  charBuf1 = buf & 255;
			  buf = buf >> 8;
			  charBuf2 = buf & 255;
			  buf = buf >> 8;
			  charBuf3 = buf & 255;
			  buf = buf >> 8;
			  charBuf4 = buf & 255;

			  //weigh likelyhoods based on arbitrary metrics
			  if (charBuf1 == ' ')
				  likelyhood += 1;
			  if (charBuf2 == ' ')
				  likelyhood += 1;
			  if (charBuf3 == ' ')
				  likelyhood += 1;
			  if (charBuf4 == ' ')
				  likelyhood += 1;

			  if ((charBuf1 >= '0') && (charBuf1 <= 'z'))
				  likelyhood += 2;
			  if ((charBuf2 >= '0') && (charBuf2 <= 'z'))
				  likelyhood += 2;
			  if ((charBuf3 >= '0') && (charBuf3 <= 'z'))
				  likelyhood += 2;
			  if ((charBuf4 >= '0') && (charBuf4 <= 'z'))
				  likelyhood += 2;

			  // bad character
			  if ((charBuf1 > '~'))
				  break;
			  if ((charBuf2 > '~'))
				  break;
			  if ((charBuf3 > '~'))
				  break;
			  if ((charBuf4 > '~'))
				  break;

			  likelyhood += 5;
		  }

		  if (max < likelyhood)
		  {
			  max = likelyhood;
			  key = j;
			  printf("Found a new max: %i\nWith key 0x%x\n", max, key);
		  }
	  }

	  if ((j - 1) / 100000000 != j / 100000000)
	  {
		  printf("Great! Making Progress! Solved %u\n", j);
	  }


  }

  printf("Good Keys: %i\n", goodKeyCount);
  printf("Best Key: 0x%x\n", key);

  return key;



};


decrypt(int key)
{
	struct stat st;
	int size, fsize;
	int* temp, result;
	int rollingkey;
	rollingkey = key;

	infile = open("output", O_RDONLY);
	if (infile < 0) { printf("input open error\n"); exit(0); }

	buf = 0;
	read(infile, &buf, 4);
	size = buf; // get plaintext size

	// ciphertext has xtra 4 bytes (size) and padding 

	stat("output", &st); fsize = st.st_size; // get ciphertext size
	if ((fsize < 8) || (size > fsize) || (size < (fsize - 8))) { printf("file size sanity check failed\n"); };

	outfile = open("output-dec", O_RDWR | O_CREAT | O_TRUNC, 0700);
	if (outfile < 0) { printf("output open error\n"); exit(0); }

	while ((n = read(infile, &buf, 4)) > 0) {
		buf = buf ^ rollingkey; // doing the reverse of encrypt
		MD5Init(&mdContext);
		MD5Update(&mdContext, &rollingkey, 4);
		MD5Final(&mdContext);
		temp = (int*)& mdContext.digest[12];
		result = *temp; // result is 32 bits of MD5 of key
		rollingkey = rollingkey ^ result; // new key

		if (size >= 4) write(outfile, &buf, 4);
		else lastbytes(outfile, size, buf);

		buf = 0;  // repeat, keep track of output size in size.
		size = size - 4;
	};
};

main(int argc, char *argv[])
{

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);


	int key = bruteForce();

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = (finish.tv_sec - start.tv_sec);
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;


	printf("Elapsed time: %f\n", elapsed);

	decrypt(key);

};
