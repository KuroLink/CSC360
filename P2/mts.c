#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#include <zconf.h>
#include <unistd.h>
#include <time.h>

#define tenthtomicro 100000
//1 tenth second * 100,000 = microsecond for usleep

pthread_mutex_t track_lock;
pthread_mutex_t crosstrack;
pthread_cond_t start;
pthread_cond_t controller;

typedef struct _station{
	pthread_mutex_t access;
	struct _station *next;

}station;

station WestH = NULL;
station WestL = NULL;
station EastH = NULL;
station EastL = NULL;

typedef struct _train{
	int id;
	int load_time;
	int cross_time;
	char *priority; 
	char dir;
	char *status;
	pthread_cond_t permission;
	struct _train *next;
}train;



void *train_function(void *ptr){
	train *t = (train *)ptr;

	pthread_mutex_lock(&track_lock);

	pthread_cond_wait(&start, &track_lock);

	pthread_mutex_unlock(&track_lock);

	pthread_mutex_lock(&crosstrack);

	if(t.dir == 'w' || t.dir == 'W'){
		printf("Train %d is ready to go West", t.id);
	}
	if(t.dir == 'e' || t.dir == 'E'){
		printf("Train %d is ready to go East", t.id);
	}

	return NULL;
}

char prevdir = '';
int num_trains;

int main(int argc, char *argv[]){
char* delim = " \n";
char buffer[10];
char line[10];
int i = 0;	

pthread_mutex_init(&track_lock, NULL);
pthread_mutex_init(&crosstrack, NULL);
pthread_cond_init(&start, NULL);.

FILE *f = fopen(argv[1], "r");

if (f == NULL){
	printf("ERROR: Invalid File");
	exit(1);
}

while(fgets(line, 10, f) != NULL){
	num_trains++;
}

rewind(f);
train train_arr[num_trains];
pthread_t trains[num_trains];

while(fgets(buffer, 10, f)!= NULL){
	train_arr[i].id = i;
	printf("%d", train_arr[i].id);
	char *direction = strtok(buffer, delim);
	train_arr[i].dir = *direction;
	int load = atoi(strtok(NULL, delim));
	train_arr[i].load_time = load;
	int cross = atoi(strtok(NULL, delim));
	train_arr[i].cross_time = cross;
	train_arr[i].status = "waiting";
	printf("%s", train_arr[i].status);

	if(train_arr[i].dir == 'W' || train_arr[i].dir == 'E'){
		train_arr[i].priority = "H";
	}
	else if (train_arr[i].dir == 'w' || train_arr[i].dir == 'e'){
		train_arr[i].priority = "L";
	}

	pthread_create(&trains[i], NULL, train_function, &train_arr[i]);



	i++;
}

pthread_cond_broadcast(&start);



for(j = 0; j < num_trains; j++){
		pthread_join(train_arr[i], NULL);
	}

}