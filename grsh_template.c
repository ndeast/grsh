#include "grsh_template.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){

	if (argc == 1) {
		interactive();	//no args - interactive mode
	}
	if (argc == 2) {
		batch(argv[1]); //args - batch mode
	}
	else{
		error(); //error
	}
	return 0;
}

void interactive(){
	//loop infinitly
	while(1) {
		//your code goes here
	}
}

void batch(char* argument){
	int redirect = 1;
	FILE *fp = fopen(argument, "r");

	//bad file exception
	if(fp == NULL){
		error();
		exit(1);
	}

	while(!feof(fp)){
		char* buffer = malloc(sizeof(char)*128);
		fgets(buffer,sizeof(buffer)*128,fp);
		buffer[strlen(buffer) -1] = '\0';
		char *args[sizeof(buffer)];

		// your code goes here

	}
	exit(0); //exit once end of file is reached
}