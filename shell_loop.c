#include "shell.h"

/**
 * hsh - main shell loop
 * @parm: the parameter & return info struct
 * @avgv: the argument vector from main()
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(ino_t *parm, char **avgv)
{
	ssize_t i = 0;
	int builtin_ret = 0;

	while (i != -1 && builtin_ret != -2)
	{
		clear_parm(parm);
		if (interactive(parm))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		i = get_input(parm);
		if (i != -1)
		{
			set_parm(parm, avgv);
			builtin_ret = find_builtin(parm);
			if (builtin_ret == -1)
				find_cmd(parm);
		}
		else if (interactive(parm))
			_putchar('\n');
		free_parm(parm, 0);
	}
	write_history(parm);
	free_parm(parm, 1);
	if (!interactive(parm) && parm->status)
		exit(parm->status);
	if (builtin_ret == -2)
	{
		if (parm->err_num == -1)
			exit(parm->status);
		exit(parm->err_num);
	}
	return (builtin_ret);
}
/**
 * find_builtin - finds a builtin command
 * @parm: the parameter & return info struct
 * Return: -1 if builtin not found,
 * 0 if builtin executed successfully,
 * 1 if builtin found but not successful,
 * 2 if builtin signals exit()
 */
int find_builtin(ino_t *parm)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(parm->argv[0], builtintbl[i].type) == 0)
		{
			parm->line_count++;
			built_in_ret = builtintbl[i].func(parm);
			break;
		}
	return (built_in_ret);
}
/**
 * find_cmd - finds a command in PATH
 * @parm: the parameter & return info struct
 * Return: void
 */
void find_cmd(ino_t *parm)
{
	char *path = NULL;
	int i, j;

	parm->path = parm->argv[0];
	if (parm->linecount_flag == 1)
	{
		parm->line_count++;
		parm->linecount_flag = 0;
	}
	for (i = 0, j = 0; parm->arg[i]; i++)
		if (!is_delim(parm->arg[i], " \t\n"))
			j++;
	if (!j)
		return;

	path = find_path(parm, _getenv(parm, "PATH="), parm->argv[0]);
	if (path)
	{
		parm->path = path;
		fork_cmd(parm);
	}
	else
	{
		if ((interactive(parm) || _getenv(parm, "PATH=")
					|| parm->argv[0][0] == '/') && is_cmd(parm, parm->argv[0]))
			fork_cmd(parm);
		else if (*(parm->arg) != '\n')
		{
			parm->status = 127;
			print_error(parm, "not found\n");
		}
	}
}
/**
 * fork_cmd - forks a an exec thread to run cmd
 * @parm: the parameter & return info struct
 * Return: void
 */
void fork_cmd(ino_t *parm)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(parm->path, parm->argv, get_environ(parm)) == -1)
		{
			free_parm(parm, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(parm->status));
		if (WIFEXITED(parm->status))
		{
			parm->status = WEXITSTATUS(parm->status);
			if (parm->status == 126)
				print_error(parm, "Permission denied\n");
		}
	}
}
