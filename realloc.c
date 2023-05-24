#include "shell.h"

/**
 * _memset - fills memory with a constant byte
 * @a: the pointer to the memory area
 * @b: the byte to fill *a with
 * @n: the amount of bytes to be filled
 * Return: (a) a pointer to the memory area s
 */
char *_memset(char *a, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		a[i] = b;
	return (a);
}
/**
 * sfree - frees a string of strings
 * @ss: string of strings
 */
void sfree(char **ss)
{
	char **c = ss;

	if (!ss)
		return;
	while (*ss)
		free(*ss++);
	free(c);
}
/**
 * _realloc - reallocates a block of memory
 * @ptr: pointer to previous malloc'ated block
 * @o_size: byte size of previous block
 * @n_size: byte size of new block
 * Return: pointer to da ol'block nameen.
 */
void *_realloc(void *ptr, unsigned int o_size, unsigned int n_size)
{
	char *p;

	if (!ptr)
		return (malloc(n_size));
	if (!n_size)
		return (free(ptr), NULL);
	if (n_size == o_size)
		return (ptr);

	p = malloc(n_size);
	if (!p)
		return (NULL);

	o_size = o_size < n_size ? o_size : n_size;
	while (o_size--)
		p[o_size] = ((char *)ptr)[o_size];
	free(ptr);
	return (p);
}

