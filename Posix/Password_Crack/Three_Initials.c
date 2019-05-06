#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

#define SALT "$6$KB$"

/******************************************************************************
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2 uppercase
  letters and a 2 digit integer. Your personalised data set is included in the
  code. 

  Compile with:
    cc -o Three_Initials Three_Initials.c -lcrypt

  If you want to analyse the results then use the redirection operator to send
  output to a file that you can view using an editor or the less utility:

    ./Three_Initials > Three_Initials.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
******************************************************************************/
int n_passwords = 4;

// Three initials passwords encrypted using EncryptSHA512.c program
char *encrypted_passwords[] = {
  "$6$KB$sAAwJ7W2tLaM1UzPmr84SNxv1AcsPhMi86KWoh81drnmKEJqNOpm67zckMjGOLUra3VTniCYHhemJL2E7XY/M.",//BGN21
  "$6$KB$HgqQ6XI8uY5jNaKN5kryFEpvA1YMIWu/.eY5nXiAbe4jN4XeEnnhgbHf20rcsYP1E29ecM58o2fk3ZEuSF3ay0",//AMD12
  "$6$KB$jevjNhqZKLm942CqziFxVtiueoIQ/OagUJ51J0xzBWi/ucwTd7UVhC5yeX6m21lpzgZ76j3hlCHWSEHa.gweO0",//PRA22
  "$6$KB$X8lfZFtwa1IHBO72O4/n0Ibp6hD3T.uNUS4/ig4SwCNzCexglqeB4fnBy3Gt6z2Lz7.bCeipeEnhnLGoRakD3/"//SHU24
};
/**
 Required by lack of standard function in C.   
*/

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

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void crack(char *salt_and_encrypted){
  int w,x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(w ='A'; w<='Z'; w++){// adding one for loop for three initials passwork crack
	  for(x='A'; x<='Z'; x++){
		for(y='A'; y<='Z'; y++){
		  for(z=0; z<=99; z++){
		    sprintf(plain, "%c%c%c%02d", w,x, y, z); 
		    enc = (char *) crypt(plain, salt);
		    count++;
		    if(strcmp(salt_and_encrypted, enc) == 0){
		      printf("#%-8d%s %s\n", count, plain, enc);
		    } else {
		      printf(" %-8d%s %s\n", count, plain, enc);
		    }
		  }
		}
	  }
	}
  printf("%d solutions explored\n", count);
}

int main(int argc, char *argv[]){
  int i;
  struct timespec start, finish;   
  long long int time_elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);
  for(i=0;i<n_passwords;i<i++) {
    crack(encrypted_passwords[i]);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9)); 


  return 0;
}
