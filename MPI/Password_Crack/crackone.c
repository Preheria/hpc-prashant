	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>
	#include <crypt.h>
	#include <mpi.h>
	#include <string.h>
	#include <time.h>

/*****************************************************************************
The variable names and the function names of this program is same as provided by the university.
The added variable and function are the only changes made to this program.
  

To compile:
     mpicc -o crack_one crack_one.c -lrt -lcrypt
     
  To run 3 processes on this computer:
    mpirun -n 3 ./crack_one 
*****************************************************************************/



	int count = 0;// Global variable declaration so they can be accessed throughout the file.
	int n_passwords = 4;
	int size, rank, pass;
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

		
  		if(size != 3) {
    		if(rank == 0) {
      		printf("This program needs to run on exactly 3 processes\n");
   		 }
  		} else {
    	if(rank ==0){
			long long int time_elapsed;
    
      		MPI_Send(&pass, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
      		MPI_Send(&pass, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);


 
		} else if(rank==1){
		  	MPI_Recv(&pass, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
		                     MPI_STATUS_IGNORE);
		    kernel_function_1();
		  }
		  else{
		  	MPI_Recv(&pass, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, 
		                     MPI_STATUS_IGNORE);
		    kernel_function_2();
		  }
		}
		 
	}
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
					printf(" %-8d%s %s\n", count, plain, enc);
					}
				}
			}
		}
	}

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
					printf(" %-8d%s %s\n", count, plain, enc);
					}
				}
			}
		}
	}
	
	struct timespec start, finish;
	
	int main(int argc, char *argv[]){
		   
		MPI_Init(NULL, NULL);
  		MPI_Comm_size(MPI_COMM_WORLD, &size);
  		MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    	clock_gettime(CLOCK_MONOTONIC, &start);
		for(int i=0;i<n_passwords;i<i++) {
    		crack(encrypted_passwords[i]);
  		} // encrypted value passed through crack() function
		
		MPI_Finalize();
		if(rank == 0){
    		long long int time_elapsed;
    		clock_gettime(CLOCK_MONOTONIC, &finish);
    		time_difference(&start, &finish, &time_elapsed);
    		printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
           (time_elapsed/1.0e9)); 
  		}
		return 0;

	}
