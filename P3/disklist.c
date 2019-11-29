#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <time.h>

int getBlocksize(char* addr) {
	int blksize=0;
	memcpy(&blksize, addr+8, 2);
	blksize=htons(blksize);
	return blksize;
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

int checkfilesize(char* addr) {
	int fsize = 0;
	memcpy(&fsize, addr+9, 4);
	fsize = ntohl(fsize);
	return fsize;
}

int modyear(char* addr) {
	int year = 0;
	memcpy(&year, addr+20, 2);
	year = ntohs(year);
	return year;
}

int modmth(char* addr) {
	int month = 0;
	memcpy(&month, addr+22, 1);
	//month = ntohs(month);
	return month;
}

int modday(char* addr) {
	int day = 0;
	memcpy(&day, addr+23, 1);
	//day = ntohs(day);
	return day;
}

int modhour(char* addr) {
	int hour = 0;
	memcpy(&hour, addr+24, 1);
	//hour = ntohs(hour);
	return hour;
}

int modmin(char* addr) {
	int min = 0;
	memcpy(&min, addr+25, 1);
	//min = ntohs(min);
	return min;
}

int modsec(char* addr) {
	int sec = 0;
	memcpy(&sec, addr+26, 1);
	//sec = ntohs(sec);
	return sec;
}


void printrootdirectory(char* addr, int rsb, int rbs, int numblks) {
	for(int i=0; i<numblks; i++){
		int fileinfo = 0;
		char* addrstrt = addr + rsb + (rbs*i);
		memcpy(&fileinfo, addrstrt, 1);

		int filesize = checkfilesize(addrstrt);
		char fname[31];
		memcpy(&fname, addrstrt+27, 31);

		int year = modyear(addrstrt);
		int mth = modmth(addrstrt);
		int day = modday(addrstrt);
		int hour = modhour(addrstrt);
		int min = modmin(addrstrt);
		int sec = modsec(addrstrt);

		if(fileinfo == 3){
			printf("F %10d %30s %d/%.2d/%.2d %.2d:%.2d:%.2d\n", filesize, fname, year, mth, day, hour, min, sec);
		}
		else if (fileinfo == 5){
			printf("D %10d %30s %d/%.2d/%.2d %.2d:%.2d:%.2d\n", filesize, fname, year, mth, day, hour, min, sec);
		}
		else continue;
	}
}

int main(int argc, char* argv[]) {
	
	if (argc != 3){
		printf("Error: Not enough parameters. Please try again");
		exit(1);
	}

	int file = open(argv[1], O_RDWR);
	char * dir = argv[2];
	char rootsym[] = "/";
	struct stat buffer;
    int status = fstat(file, &buffer);

    char* address=mmap(NULL, buffer.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);

    int blocksize = getBlocksize(address);
	int rdirstrt = Rootdirstrt(address);    
	int rdirblk = Rootdirblk(address);

	int rtstrtblk = rdirstrt * blocksize;
	int rtblksize = blocksize / rdirblk;

	//implemented for root directory only
	int dircheck = strcmp(dir, rootsym);
	if(dircheck == 0){
		printrootdirectory(address, rtstrtblk, rtblksize, rdirblk);
	}
	else {
		printf("Not root directory.");
		exit(1);
	}
	

	munmap(address,buffer.st_size);
 	close(file); 
}