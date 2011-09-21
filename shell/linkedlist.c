#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "linkedlist.h"

struct pathnode *init_path()
{
	return NULL;
}

/* Adds a directory given in the path command to the path variable */
struct pathnode *add_to_path(struct pathnode *startnode, const char *newpath)
{
	struct pathnode *ret_val;
	ret_val = startnode;
	if (startnode == NULL) {
		startnode = (struct pathnode *) malloc(sizeof(struct pathnode));
		handle_failure(startnode);
		startnode->value = (char *) malloc(strlen(newpath) + 1);
		handle_failure(startnode->value);
		strcpy(startnode->value, newpath);
		startnode->next = NULL;
		ret_val = startnode;
		return ret_val;

	} else {
		while (startnode->next != NULL)
			startnode = startnode->next;

		startnode->next = (struct pathnode *) malloc(
				sizeof(struct pathnode));
		handle_failure(startnode->next);
		startnode = startnode->next;
		startnode->value = (char *) malloc(strlen(newpath) + 1);
		handle_failure(startnode->value);
		strcpy(startnode->value, newpath);
		startnode->next = NULL;
		return ret_val;
	}
}

/* Deletes a given directory from the path variable if it exits. Otherwise,
 * it displays an error message.
 * */
struct pathnode *del_from_path(struct pathnode *startnode, const char *path)
{
	struct pathnode *temp;
	struct pathnode *ret_val;
	ret_val = startnode;
	temp = startnode;
	if (startnode == NULL) {
		return NULL;

	} else if (!strcmp(startnode->value, path)) {
		temp = startnode->next;
		startnode->next = NULL;
		free(startnode);
		ret_val = temp;
		return ret_val;

	} else {

		while (startnode != NULL && strcmp(startnode->value, path)) {
			temp = startnode;
			startnode = startnode->next;
		}

		if (startnode == NULL) {
			printf("No such element\n");
			return ret_val;

		} else {
			temp->next = startnode->next;
			free(startnode);
			return ret_val;
		}
	}
}

void printlist(struct pathnode *startnode)
{
	if (startnode == NULL) {
		printf("The path list is empty\n");

	} else {
		while (startnode != NULL) {
			printf("%s:", startnode->value);
			startnode = startnode->next;
		}
		printf("\n");
	}
}

void handle_failure(void *ptr)
{
	if (ptr == NULL) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}

/*
 * Searches the directory list stores in the "path" of this shell to return
 * the actual file path of the given command.
 *
 * Returns NULL if no directory in path list is found which has the command cmd.
 *
 * NOTE: The caller should free the string returned
 *
 * */
char *find_absolute_path(struct pathnode *path_head, const char *cmd)
{
	struct pathnode *tmp = path_head;
	char *dirname;
	int dirlen;
	char *file_path;

	while (tmp != NULL) {
		dirname = tmp->value;
		dirlen = strlen(dirname);

		if (dirname[dirlen - 1] == '/') {

			file_path = (char *) malloc(
					strlen(dirname) + strlen(cmd) + 1);
			strcat(file_path, dirname);
			strcat(file_path, cmd);

		} else {
			/* 1 for '/' and other '\0' */
			file_path = (char *) malloc(
					strlen(dirname) + strlen(cmd) + 2);
			strcat(file_path, dirname);
			strcat(file_path, "/");
			strcat(file_path, cmd);

		}

		if (check_file_exists(file_path) == 1)
			return file_path;

		free(file_path);
		tmp = tmp->next;
	}

	return NULL;
}

/* Returns 1 if the file paths exists or valid */
int check_file_exists(const char *file_path)
{
	struct stat sb;
	int ret_val = stat(file_path, &sb);

	if (ret_val == 0)
		return 1;

	return -1;
}

