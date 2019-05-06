	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <crypt.h>
	#include <pthread.h>
	#include <string.h>
/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o CrackAZ99-With-Posix CrackAZ99-With-Posix.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./CrackAZ99-With-Posix > CrackAZ99-With-Posix.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
	int count = 0;// Global variable declaration so they can be accessed throughout the file.
	int n_passwords = 4;

	char *encrypted_passwords[] = {
		"$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",
		"$6$KB$iRfcI5qhmJ65feS0jAh2VgwUU6fLOShgNlv3UpjnOO7hfPAm/hSSnQUBxuYrFjZkNlaNPxdU9YsKs5qmJ49Kp0",
		"$6$KB$rx80JUfdamTteOinUYq.mWZHjfTxouPq5nK.I.J90L5exCuADDGR8aO820Svb3tIz3Gt7QjWskT6iNgTy5WdS1",
		"$6$KB$QHHbFXTX/MDUgnQpchfZu9nakAFKElNesOowHFTevfOQJj/RlK4jAItpetpy3/ji4b5H98AvICKJ8RnZG6ZnX."
	};


	void substr(char *dest, char *src, int start, int length){
	  memcpy(dest, src + start, length);
	  *(dest + length) = '\0';
	}

	int time_difference(struct timespec *start, struct timespec *finish,
		                long long int *difference) {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) {
		ds--;
		dn += 1000000000; 
	  } 
	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
	}

	char salt[7],*encrypted;

	typedef struct encry
	{
		int start;
		int stride;
	} encry;

	void crack(char *salt_and_encrypted){ // decrypts the encrypted value

		substr(salt, salt_and_encrypted, 0, 6);

		pthread_t thread_1, thread_2;

		encrypted = salt_and_encrypted;

		void *kernel_function_1();
		void *kernel_function_2();

		pthread_create(&thread_1, NULL, kernel_function_1, NULL);// thread_1 thread created
		pthread_create(&thread_2, NULL, kernel_function_2, NULL);//thread_2 thread created

		pthread_join(thread_1, NULL);// prioritizes thread_1 thread over thread_2
		pthread_join(thread_2, NULL);
	}

	// this function checks passwords ranging from AA00 - MZ99
	void *kernel_function_1(){
		int x,y,z;
		char plain[7],*enc;
		for(x='A'; x<='M'; x++){
			for(y='A'; y<='Z'; y++){
				for(z=0; z<=99; z++){
				sprintf(plain, "%c%c%02d", x, y, z);
				enc=(char *) crypt(plain,salt); // encrypts each and every values obtained through loop
				count++;
					if(strcmp(encrypted, enc) == 0){ // comparision between two encrypted values if matched returns 0
					printf("#%-8d%s %s\n", count, plain, enc);
					} else {
					//printf(" %-8d%s %s\n", count, plain, enc);
					}
				}
			}
		}
	}
	
	//this function checks passwords ranging from NA00 - ZZ99
	void *kernel_function_2(){
		int x,y,z;
		char plain[7],*enc;
		for(x='N'; x<='Z'; x++){
			for(y='A'; y<='Z'; y++){
				for(z=0; z<=99; z++){
				sprintf(plain, "%c%c%02d", x, y, z);
				enc=(char *) crypt(plain,salt); // encrypts each and every values obtained through loop
				count++;
					if(strcmp(encrypted, enc) == 0){ // comparision between two encrypted values if matched returns 0
					printf("#%-8d%s %s\n", count, plain, enc);
					} else {
					//printf(" %-8d%s %s\n", count, plain, enc);
					}
				}
			}
		}
	}

	int main(int argc, char *argv[]){
		struct timespec start, finish;   
 		long long int time_elapsed;
    	clock_gettime(CLOCK_MONOTONIC, &start);
		for(int i=0;i<n_passwords;i<i++) {
    	crack(encrypted_passwords[i]);
  		} // encrypted value passed through crack() function
		printf("%d solutions explored\n", count);
		clock_gettime(CLOCK_MONOTONIC, &finish);
  		time_difference(&start, &finish, &time_elapsed);
  		printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9));
		return 0;
	}
