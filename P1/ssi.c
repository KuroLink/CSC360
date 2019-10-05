#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>

int main()
{
	char *name;
       	name=(char *)malloc(20*sizeof(char));
	cuserid(name);
	char hostname[20];
	int rc = gethostname(hostname, sizeof(hostname));
	char cwd[50];
	getcwd(cwd, sizeof(cwd));
	//printf("SSI: %s@%s %s > ", name, hostname, cwd);
	while(1){
		printf("SSI: %s@%s %s > ", name, hostname, cwd);
	char str[100];
		fgets(str, 25, stdin);
		//printf("input is %s\n", str);
		//char* token = strtok(str, " \n");
		char* args[50];	
		//printf("first token is %s\n", token);
		//printf("argv array is %d", argv[0]);
		args[0] = strtok(str, " \n");
		int i=0;
		while(args[i]!=NULL){
			args[i+1]=strtok(NULL, " \n");
			i++;
		}
		//args[i] = "NULL";
		//printf("First part of string is %s\n", args[0]);
		//execvp(args[0], args);
		//(strcmp(args[0], "exit")==0){
		//	exit(0);
		//}
		pid_t pid;
		int status;
		if((pid = fork()) <0) {
			printf("ERROR\n");
			exit(1);
		}
		else if (pid == 0) {
				
				//printf("My process ID : %d\n", getpid());
			
			if(strcmp(args[0], "cd") ==0){
				//printf("inside cd function and %s\n", args[1]);
				if(args[1] == 0){
					chdir(getenv("HOME"));
					getcwd(cwd, sizeof(cwd));
				}
				else if(strcmp(args[1], "..") ==0){
					chdir("..");
					getcwd(cwd, sizeof(cwd));
					//printf("SSI: %s@%s %s > ", name, hostname, cwd);
				}
				else if(strcmp(args[1], "~")==0){	
					chdir(getenv("HOME"));
					getcwd(cwd, sizeof(cwd));
					//printf("SSI: %s@%s %s > ", name, hostname, cwd);
				}
				
			}	
			else if(execvp(args[0], args) < 0) {
				printf("ERROR exe\n");
				exit(1);
				}
			}
		
		else {
			while(wait(&status) != pid);
			}
		
		//execvp(args[0], args);
		//memset(args, 0, 50);	
	}		
}
