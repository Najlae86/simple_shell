#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <limits.h>

int _putchar(char c);
void _puts(char *str);
int _strlen(char *s);
char *_strdup(char *str);
char *concat_all(char *name, char *sep, char *value);

char **splitstring(char *str, const char *delim);
void execute(char **argv);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);


extern char **environ;

/**
 * struct list_path - Linked list containing PATH directories
 * @dir: directory in path
 * @p: pointer to next node
 */
typedef struct list_path
{
	char *dir;
	struct list_path *p;
} list_path;


char *_getenv(const char *name);
list_path *add_node_end(list_path **head, char *str);
list_path *linkpath(char *path);
char *_which(char *filename, list_path *head);

/**
 * struct mybuild - pointer to function with corresponding buildin command
 * @name: buildin command
 * @func: execute the buildin command
 */
typedef struct mybuild
{
	char *name;
	void (*func)(char **);
} mybuild;

void(*checkbuild(char **arv))(char **arv);
int _atoi(char *s);
void exitt(char **arv);
void env(char **arv);
void _setenv(char **arv);
void _unsetenv(char **arv);
void freearv(char **arv);
void free_list(list_path *head);

/* memory.c */
int Pfree(void **ptr);
/* shell_loop.c */
int hsh(ino_t *parm, char **avgv);
int find_builtin(ino_t *parm);
void find_cmd(ino_t *parm);
void fork_cmd(ino_t *parm);
/* parser.c */
int the_cmd(ino_t *ino, char *path);
char *dup_char(char *pathstr, int s, int e);
char *find_path(ino_t *info, char *pathstr, char *cmd);
/* realloc.c */
char *_memset(char *a, char b, unsigned int n);
void sfree(char **ss);
void *_realloc(void *ptr, unsigned int o_size, unsigned int n_size);
/* string1.c */
int _strlen(char *s);
int _strcmp(char *s1, char *s2);
char *starts_with(const char *s_s, const char *sub_s);
char *_strcat(char *dest, char *src);
/* string2.c */
char *_strcpy(char *dest, char *src);
char *_dupstr(const char *str);
void _puts(char *str);
int _putchar(char c);
/* tokenizer.c */
char **str_to_w1(char *str, char *d);
char **str_to_w2(char *str, char d);
/* vars.c */
int the_chain(ino_t *parm, char *buf, size_t *a);
void check_chain(ino_t *parm, char *buf, size_t *a, size_t s, size_t len);
int rep_alias(ino_t *parm);
int rep_vars(ino_t *parm);
int rep_string(char **old, char *new);
list_path *node_starts_with(list_path *node, char *p, char c);
/* for r/w buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

int is_delim(char c, char *delim);

#endif
