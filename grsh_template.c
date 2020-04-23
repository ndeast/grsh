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
		line = read_line();
		args = parse_line(line);
		status = exec_args(args);

		free(line);
		free(args);
	}
}

char *read_line(void) {
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

char **parse_line(char *line) {
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

int exec_args(char** args) {
	//fork a chile
	pid_t pid = fork();

	if (pid == 0) {
		if (execvp(args[0], args) < 0) {
			printf("\nCould not execute command\n");
		}
		exit(0);
	} else if (pid == -1) {
		printf("\nFailed To Fork");
	} else {
		wait(NULL);
		return 1;
	}
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