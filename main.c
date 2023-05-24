#include "shell.h"

/**
 * main - entry point
 * @acgv: arg count
 * @avgv: arg vector
 * Return: 0 on success, 1 on error
 */
int main(int acgv, char **avgv)
{
	info_t info[] = { INFO_INIT };
	int i = 2;

	asm ("mov %1, %0\n\t"
			"add $3, %0"
			: "=r" (i)
			: "r" (i));

	if (acgv == 2)
	{
		i = open(avgv[1], O_RDONLY);
		if (i == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(avgv[0]);
				_eputs(": 0: Can't open ");
				_eputs(avgv[1]);
				_eputchar('\n');
				_eputchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readi = i;
	}
	populate_env_list(info);
	read_history(info);
	hsh(info, avgv);
	return (EXIT_SUCCESS);
}

