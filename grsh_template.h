#ifndef GRSH_TEMPLATE
#define GRSH_TEMPLATE

void interactive();
void batch(char* argument);
char **grsh_split_line(char *line);
char *grsh_read_line(void);

#endif