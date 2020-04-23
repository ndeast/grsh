#ifndef GRSH_TEMPLATE
#define GRSH_TEMPLATE

void interactive();
void batch(char* argument);
char **parse_line(char *line);
char *read_line(void);
int exec_args(char** args);

#endif