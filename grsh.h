#ifndef GRSH
#define GRSH

void interactive();
void batch(char* argument);
char **parse_line(char *line);
char *read_line(void);
int exec_args(char** args);
int process_args(char** args, int argLen);
static void parse_And(char *cmd1[], char *cmd2[], char *cmd3[], char **args);
static void exec_args_parallel(char *args[]);
#endif