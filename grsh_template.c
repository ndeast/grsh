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
		//error(); //error
	}
	return 0;
}

void interactive(){
	char *line;
	char **args;
	int status;
	//loop infinitly
	while(1) {
		printf("> ");
		line = grsh_read_line();
		args = grsh_split_line(line);
		// status = grsh_execute(args);

		free(line);
		free(args);
	}
}

char *grsh_read_line(void) {
	char *line = NULL;
	ssize_t buffer = 0;

	if (getline(&line, &buffer, stdin) == -1) {
		if (feof(stdin)) {
			exit(0);
		} else {
			exit(1);
		}
	}
	return line;
}

char **grsh_split_line(char *line) {
	int buffer = 64, position = 0;
	char **tokens = malloc(buffer * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf (stderr, "grsh allocation error\n");
		exit(1);
	}

	token = strtok(line, " \t\r\n\a");
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= buffer) {
			buffer += 64;
			tokens = realloc(tokens, buffer * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "grsh allocation error\n");
				exit(1);
			}
		}
		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return tokens;
}

void batch(char* argument){
	int redirect = 1;
	FILE *fp = fopen(argument, "r");

	//bad file exception
	if(fp == NULL){
		// error();
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