#include "shell.h"
/**
*bfree - freees pointer && makes adress = null
*@ptr: adress of the pointer to be free
*Return: success 0, freed 1
*/
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

