#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//Austin Riddick


// notes: AMD 1700x has L1: 768k, L2: 4MB, L3: 16MB
int main(int argc, char *agrv[]){
	cacheSize();
	memTime();
	return 0;
}
void swap(long *p, long *q){
    long t;
    t=*p;
    *p=*q;
    *q=t;
}

void sort(long a[], int n){
    int i, j, temp;
    for (i =0; i<n-1; i++){
        for(j=0;j < n-i-1; j++){
            if(a[j] > a[j+1]){
                swap(&a[j],&a[j+1]);
            }
        }
    }
}
void cacheSize(){
    int *buffer = malloc(4096); // something smaller than my L1 cache, in this case 4k (AMD 1700x has 768k)
    int *other_buffer = malloc(4096);
    memcpy(buffer, other_buffer, 4096);
    memcpy(other_buffer, buffer, 4096); // it's in L1 now
    int stride = 4177; //prime number bigger than 4096
    int* ptr = buffer;
    int k = 1;
    int time;
    struct timespec before, after;
    long greatestTime = 0;
    long timeArray[21];
    int cache = 0;
    int blockSize = 0;
    int numBlocks = 2;
    for (int n = 0; n<21; n++) {
	    blockSize = k;
	    clock_gettime(CLOCK_MONOTONIC, &before);
	    other_buffer = malloc(blockSize);///4);
	    clock_gettime(CLOCK_MONOTONIC, &after);
	    free(other_buffer);
	    long timeElapsed = (after.tv_nsec - before.tv_nsec);
	    timeArray[n] = timeElapsed;
	    if(greatestTime < timeElapsed) {
                    if(greatestTime != 0 && timeElapsed > greatestTime*2) {
                            break;
		    }
		    greatestTime = timeElapsed;
	    }
	    k *= 2;
    }
    blockSize *= 8;
    greatestTime = after.tv_nsec - before.tv_nsec;
//    printf("greatest: %ld\n", greatestTime);
    printf("Block Size: %dKB\n", blockSize/1024);
    for(int n = 0; n < 50; n++) {
	    clock_gettime(CLOCK_MONOTONIC, &before);
            other_buffer = malloc((blockSize*numBlocks));
            clock_gettime(CLOCK_MONOTONIC, &after);
	    free(other_buffer);
	    long timeElapsed = (after.tv_nsec - before.tv_nsec);
	    timeArray[n] = timeElapsed;
//	    printf("Time: %ld\t", timeArray[n]);
	    if(greatestTime < timeElapsed && greatestTime != 0 && timeElapsed > greatestTime*3.5) {
                            break;
	    }
	    numBlocks++; 
//    printf("Cache Size: %dKB\n", blockSize*numBlocks/1024);
    }
    printf("FINAL Cache Size: %dKB\n", blockSize*(numBlocks*4)/1024);
}
int memTime(void){
    srand(0); // seed my random numbers
    
   
    double cpu_time_used;
    int bigArray[2000000];
    long timeArray[2000];
    int i;
    //clockid_t clk_id;
    for (i = 0; i<2000000; i++){
        bigArray[i] = rand();
    }
    //clk_id = CLOCK_MONOTONIC;
    // loop through a bunch of times to get a good set of data
    for (int q=0; q<2000; q++){
        //clk_id = CLOCK_MONOTONIC; 
        struct timespec before;  
        clock_gettime(CLOCK_MONOTONIC, &before); // like saying timer.start()
        int randBound = (rand() % (2000000-1)); // for whatever reason sizeof(bigArray) refuses to work
        int x = bigArray[randBound]; // access array in main mem
        struct timespec after;
        clock_gettime(CLOCK_MONOTONIC, &after); //like saying timer.stop()
        //result = clock_gettime(clk_id, &tp);
        //printf("%d \n", x);
        //printf("%ld sec, %ld nanoseconds elapsed \n", after.tv_sec-before.tv_sec, after.tv_nsec-before.tv_nsec);
        long timeElapsed = (after.tv_nsec-before.tv_nsec);
        timeArray[q] = timeElapsed;
        //printf("timeArray: %ld", timeArray[q-1]);
    }
    //printf("BigArray 15: %d\n", bigArray[15]);
    //printf("timeArray: %ld\n", timeArray[15]);
    int z = 2000;
    //printf("timearray size: %d\n", z);
    sort(timeArray, z);

    z = (z+1) / 2 -1;
    printf("Median RAM access time: %d nanoseconds", timeArray[z]);
    return 0;

}
