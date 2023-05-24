#include "shell.h"

/**
 * the_chain - test if current char in buffer is a chain delimeter
 * @parm: the parameter struct
 * @buf: the char buffer
 * @a: address of current position in buffer
 * Return: 1 if chain delimeter, 0 otherwise
 */
int the_chain(ino_t *parm, char *buf, size_t *a)
{
	size_t i = *a;

	if (buf[i] == '|' && buf[i + 1] == '|')
	{
		buf[i] = 0;
		i++;
		parm->cmd_buf_type = CMD_OR;
	}
	else if (buf[i] == '&' && buf[i + 1] == '&')
	{
		buf[i] = 0;
		i++;
		parm->cmd_buf_type = CMD_AND;
	}
	else if (buf[i] == ';') /* found end of this command */
	{
		buf[i] = 0; /* replace semicolon with null */
		parm->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*a = i;
	return (1);
}
/**
 * check_chain - checks we should continue chaining based on last status
 * @parm: the parameter struct
 * @buf: the char buffer
 * @a: address of current position in buffer
 * @s: starting position in buffer
 * @len: length of buffer
 * Return: Void
 */
void check_chain(ino_t *parm, char *buf, size_t *a, size_t s, size_t len)
{
	size_t i = *a;

	if (parm->cmd_buf_type == CMD_AND)
	{
		if (parm->status)
		{
			buf[s] = 0;
			i = len;
		}
	}
	if (parm->cmd_buf_type == CMD_OR)
	{
		if (!parm->status)
		{
			buf[s] = 0;
			i = len;
		}
	}

	*a = i;
}
/**
 * rep_alias - replaces an aliases in the token string
 * @parm: the parameter struct
 * Return: 1 if replaced, 0 otherwise
 */
int rep_alias(ino_t *parm)
{
	int i;
	list_path *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(parm->alias, parm->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}
/**
 * rep_vars - replaces vars in the token string
 * @parm: the parameter struct
 * Return: 1 if replaced, 0 otherwise
 */
int rep_vars(ino_t *parm)
{
	int i = 0;
	list_path *node;

	for (i = 0; parm->argv[i]; i++)
	{
		if (parm->argv[i][0] != '$' || !parm->argv[i][1])
			continue;
		if (!_strcmp(parm->argv[i], "$?"))
		{
			rep_string(&(parm->argv[i]),
					_strdup(convert_number(parm->status, 10, 0)));
			continue;
		}
		if (!_strcmp(parm->argv[i], "$$"))
		{
			rep_string(&(parm->argv[i]),
					_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(parm->env, &info->argv[i][1], '=');
		if (node)
		{
			rep_string(&(parm->argv[i]),
					_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		rep_string(&parm->argv[i], _strdup(""));
	}
	return (0);
}
/**
 * rep_string - replaces string
 * @old: address of old string
 * @new: new string
 * Return: 1 if replaced, 0 otherwise
 */
int rep_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
