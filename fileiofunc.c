#include "shell.h"

/**
* get_history_file - gets the history file
*@info: parameter struct
*Return: allocated string contain history file
*/
char *get_history_file(info_t *info)
{
	char *buffer, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	buffer = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	_strcpy(buffer, dir);
	_strcat(buffer, "/");
	_strcat(buffer, HIST_FILE);
	return (buffer);
}

/**
* write_history - creates a file or appended to an exiting file
*@info: parameter struct
*Return: 1 on success, -1 otherwise
*/
int write_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
* read_history - reads history from file
*@info: parameter struct
*Return: 1 on success, -1 otherwise
*/
int read_history(info_t *info)
{
	int i, last = 0, numline = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buffer = NULL, *filename = get_history_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buffer = malloc(sizeof(char) * (fsize + 1));
	if (!buffer)
		return (0);
	rdlen = read(fd, buffer, fsize);
	buffer[fsize] = 0;
	if (rdlen <= 0)
		return (free(buffer), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			build_history_list(info, buffer + last, numline++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(info, buffer + last, numline++);
	free(buffer);
	info->histcount = numline;
	while (info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(info->history), 0);
	renumber_history(info);
	return (info->histcount);
}

/**
* build_history_list - adds entry to a history linked list
*@info: parameter struct
*@buffer: buffer
*@numline: history numline , histcount
*Return: always 0
*/
int build_history_list(info_t *info, char *buffer, int numline)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_node_end(&node, buffer, numline);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
* renumber_history - renumbers the history linked list after changes
*@info: parameter struct
*Return: 1 on success, -1 otherwise
*/
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}

