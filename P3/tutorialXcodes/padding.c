#include<stdio.h>

struct __attribute__((__packed__)) info{
	char c;
	long i;
	short j;
};

void main(){
	printf("size of struct: %ld\n", sizeof(struct info));
}
