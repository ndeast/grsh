#include "grsh_template.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

bool is_and = false;
bool is_redirect = false;

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
	int status, execFlag, argLen;
	//loop infinitly
	while(1) {
		printf("> ");
		line = read_line();
		args = parse_line(line);
		//store first arg element as arglength and copy rest of values to new array
		sscanf(args[0], "%d", &argLen);
		char* argList[argLen]; 
		for(int i = 0; i < argLen + 1; i++) {
			argList[i] = args[i + 1];
		}

		execFlag = process_args(argList, argLen);
		if (execFlag == 1) {
			status = exec_args(argList);
		} else if (execFlag == 2) {
			printf("\nparallel args\n");
		}

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
	//reserve position 0 for array length
	int buffer = 64, position = 1;
	char **tokens = malloc(buffer * sizeof(char*));
	char *token;
	char str[10];

	if (!tokens) {
		fprintf (stderr, "grsh allocation error\n");
		exit(1);
	}
	tokens[0] = "0"; //store placeholder in token array
	token = strtok(line, " \t\r\n\a");
	while (token != NULL) {
		tokens[position] = token;
		position++;
		if (position - 1 >= buffer) {
			buffer += 64;
			tokens = realloc(tokens, buffer * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "grsh allocation error\n");
				exit(1);
			}
		}
		if(strcmp(token, "&") == 0) {
			is_and = true;
		}
		token = strtok(NULL, " \t\r\n\a");
	}
	sprintf(str, "%d", position - 1); //convert position to a string
	tokens[0] = str;
	tokens[position] = NULL;


	return tokens;
}

int exec_args(char** args) {
	//fork a child
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

static void exec_args_parallel(char *args[]) {
	//fork a child
	pid_t pid = fork();

	if (pid == 0) {
		if (execvp(args[0], args) < 0) {
			printf("\nCould not execute command\n");
		}
	} else if (pid == -1) {
		printf("\nFailed To Fork");
	}
	return;
}

int builtin_handler(char** args) {
	int numCmds = 3, i, y = 0;
	char* CmdList[numCmds];
	char cwd[1024];

	CmdList[0] = "exit";
	CmdList[1] = "cd";
	CmdList[2] = "path";

	for (i = 0; i < numCmds; i++) {
		if (strcmp(args[0], CmdList[i]) == 0) {
			y = i + 1;
			break;
		}
	}

	switch (y) {
		case 1:
			printf("Exiting\n");
			exit(0);
		case 2:
			chdir(args[1]);
			getcwd(cwd, sizeof(cwd));
			// printf("Current Dir: %s\n", cwd);
			return 1;
		case 3:
			getcwd(cwd, sizeof(cwd));
			printf("Current Dir: %s\n", cwd);
			return 1;
		default:
			break;
	}
	return 0;

}

static void parse_And(char *cmd1[], char *cmd2[], char *cmd3[], char **args) {
	int a, b, c;
	for (a = 0; *args[a] != '&'; a++) {
		cmd1[a]= args[a];
	}
	cmd1[a++] = '\0';

	for (b = 0; args[a] != NULL; a++, b++) {
		if(*args[a] != '&') {
			cmd2[b] = args[a];
		} else {
			break;
		}	
	}
	cmd2[b] = '\0';

	if (args[a] != NULL) {
		a++;
		for (c = 0; args[a] != NULL; a++, c++) {
			cmd3[c] = args[a];
		}
		cmd3[c] = '\0';
	} else {
		cmd3[0] = NULL;
	}
}

int handle_And(char **args) {
	int status;
	char *cmd1[20], *cmd2[20], *cmd3[20];
	parse_And(cmd1, cmd2, cmd3, args);
	is_and = false;
	if(cmd1[0] != NULL && !builtin_handler(cmd1)) {
		exec_args_parallel(cmd1);
	}
	if(cmd2[0] != NULL && !builtin_handler(cmd2)) {
		exec_args_parallel(cmd2);
	}
	if(cmd3[0] != NULL && !builtin_handler(cmd3)) {
		exec_args_parallel(cmd3);
	}
	do {
		status = wait(NULL);
	} while (status > 0);

	return 1;
}

int process_args(char** args, int argLen) {
	if (is_and) {
		int status = handle_And(args);
		if (status == 1) {
			return 0;
		}
	}
	if (builtin_handler(args)) {
		return 0;
	} else {
		return 1;
	} 
}

void batch(char* argument) {
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