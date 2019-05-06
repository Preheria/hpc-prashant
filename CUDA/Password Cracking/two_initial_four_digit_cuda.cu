#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>

/****************************************************************************
  This program gives an example of a poor way to implement a password cracker
  in CUDA C. It is poor because it acheives this with just one thread, which
  is obviously not good given the scale of parallelism available to CUDA
  programs.
  
  The intentions of this program are:
    1) Demonstrate the use of __device__ and __global__ functions
    2) Enable a simulation of password cracking in the absence of library 
       with equivalent functionality to libcrypt. The password to be found
       is hardcoded into a function called is_a_match.   

  Compile and run with:
    nvcc -o cuda_crack cuda_crack.cu
    ./cuda_crack
   
  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

/****************************************************************************
  This function returns 1 if the attempt at cracking the password is 
  identical to the plain text password string stored in the program. 
  Otherwise,it returns 0.
*****************************************************************************/


__device__ void is_a_match(char *attempt) {
	char *plain_password[] = {(char *)"AV7211",(char *)"FR2422",(char *)"HS3134",(char *)"TD1434"};

	for(int x = 0; x < 4; x++){
		  	char *a = attempt;
		  	char *p = plain_password[x];
		  
		  	while(*a == *p) {
				if(*a == '\0') {
			  	printf("password found: %s\n", plain_password[x]);
			  	break;
				}
				a++;
				p++;
		  	}
		}

}

/****************************************************************************
  The kernel function assume that there will be only one thread and uses 
  nested loops to generate all possible passwords and test whether they match
  the hidden password.
*****************************************************************************/

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

__global__ void  kernel() {
char w,x,y,z;
  
  char password[7];
  password[6] = '\0';

int i = blockIdx.x+65;
int j = threadIdx.x+65;
char firstAlp = i; 
char secondAlp = j; 
    
password[0] = firstAlp;
password[1] = secondAlp;
	for(w='0'; w<='9'; w++){
	  for(x='0'; x<='9'; x++){
	   for(y='0'; y<='9'; y++){
	     for(z='0'; z<='9'; z++){
	        password[2] = w;
	        password[3] = x;
	        password[4] = y;
	        password[5] = z; 
			// passing password values to match with required password value
	      	is_a_match(password);
	   }
	}
	}
	}

}

int main() {
	struct timespec start, finish;   
  	long long int time_elapsed;

	// defining block and grid dimensions of (26(x),1(y),1(z)) and (26 (x), 1(y), 1(x))
	dim3 gd(26, 1, 1); 
  	dim3 bd(26, 1, 1);
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	// <<<gd,bd>>> represents grid and block dimensions respectively
  	kernel <<<gd, bd>>>();

	//blocks until the device has completed all preceding requested task
  	cudaThreadSynchronize();

	clock_gettime(CLOCK_MONOTONIC, &finish);
  	time_difference(&start, &finish, &time_elapsed);
  	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
         (time_elapsed/1.0e9)); 
  	return 0;
}


