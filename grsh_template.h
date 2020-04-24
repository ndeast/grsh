#ifndef GRSH_TEMPLATE
#define GRSH_TEMPLATE

void interactive();
void batch(char* argument);
char **parse_line(char *line);
char *read_line(void);
int exec_args(char** args);
int process_args(char** args, int argLen);
static void parse_And(char *left[], char *right[], char **args);
static void exec_args_parallel(char *args[]);
#endif