#include "shell.h"

/**
 * the_cmd - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 * Return: 1 if true, 0 otherwise
 */
int the_cmd(ino_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st))
		return (0);
	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}
/**
 * dup_char - duplicates characters
 * @pathstr: the PATH string
 * @s: starting index
 * @e: stopping index
 * Return: pointer to new buffer
 */
char *dup_char(char *pathstr, int s, int e)
{
	static char buf[1024];
	int i = 0, j = 0;

	for (j = 0, i = s; i < e; i++)
		if (pathstr[i] != ':')
			buf[j++] = pathstr[i];
	buf[j] = 0;
	return (buf);
}
/**
 * find_path - finds this command in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the command to find
 * Return: full path of command if found or NULL
 */
char *find_path(ino_t *info, char *pathstr, char *cmd)
{
	int i = 0, now = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (the_cmd(info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_char(pathstr, now, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (the_cmd(info, path))
				return (path);
			if (!pathstr[i])
				break;
			now = i;
		}
		i++;
	}
	return (NULL);
}

