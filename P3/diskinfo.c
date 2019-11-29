#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

/*University of Victoria
  CSC360 Operating Systems
  Charles Yang
  Programming Assignment 3
  */

int getBlocksize(char* addr) {
	int blksize=0;
	memcpy(&blksize, addr+8, 2);
	blksize=htons(blksize);
	return blksize;
}

int getBlockcount(char* addr) {
	int blk=0;
	memcpy(&blk, addr+10, 4);
	blk=htonl(blk);
	return blk;
}

int FATstrt(char* addr) {
	int start=0;
	memcpy(&start, addr+14, 4);
	start=htonl(start);
	return start;
}

int FATblck(char* addr) {
	int fblock=0;
	memcpy(&fblock, addr+18, 4);
	fblock=htonl(fblock);
	return fblock;
}

int Rootdirstrt(char* addr) {
	int rds=0;
	memcpy(&rds, addr+22, 4);
	rds=htonl(rds);
	return rds;
}

int Rootdirblk(char* addr) {
	int rdb=0;
	memcpy(&rdb, addr+26, 4);
	rdb=htonl(rdb);
	return rdb;
}

int main(int argc, char* argv[]) {
	int file = open(argv[1], O_RDWR);
	struct stat buffer;
    int status = fstat(file, &buffer);

    char* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    int blocksize = getBlocksize(address);
    int block = getBlockcount(address);
    int fstrt = FATstrt(address);
    int fblk = FATblck(address);
    int rdirstrt = Rootdirstrt(address);
    int rdirblk = Rootdirblk(address);

    int FATstart = blocksize * fstrt;
    int FATend = blocksize * fblk;
    int FATblockcount = FATstart + FATend;	

    int frblk = 0;
    int resblk = 0;
    int allblk = 0;

    for(int i=FATstart; i<FATblockcount; i+=4){
    	int FATentry = 0;
    	memcpy(&FATentry, address+i, 4);
    	FATentry = htonl(FATentry);
    	if (FATentry == 0) {
    		frblk++;
    	}
    	else if (FATentry == 1){
    		resblk++;
    	}
    	else {
    		allblk++;
    	}
    }

    printf("Super block information:\n");
    printf("Block size: %d\n", blocksize);
    printf("Block count: %d\n", block);
    printf("FAT starts: %d\n", fstrt);
    printf("FAT blocks: %d\n", fblk);
    printf("Root directory start: %d\n", rdirstrt);
    printf("Root directory blocks: %d\n\n", rdirblk);
    printf("FAT information\n");
    printf("Free Blocks: %d\n", frblk);
    printf("Reserved Blocks: %d\n", resblk);
    printf("Allocated Blocks: %d\n", allblk);
    
 	munmap(address,buffer.st_size);
 	close(file);   
}