#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <errno.h>
#include <unistd.h>
#include "stack.h"

/* Global variables */
struct stacknode *top;
struct stacknode *bottom;

void initstack()
{
	top = NULL;
	bottom = NULL;
}

void push(const char *newdata)
{
	struct stacknode *newnode = (struct stacknode *) malloc(
			sizeof(struct stacknode));
	handle_failure_stack(newnode);
	newnode->data = (char *) malloc(strlen(newdata) + 1);
	handle_failure_stack(newnode->data);
	strcpy(newnode->data, newdata);
	newnode->next = NULL;

	/* if stack is empty */
	if (top == NULL) {
		top = newnode;
		bottom = newnode;

	} else {
		/* stack grows left side */
		newnode->next = top;
		top = newnode;
	}
}

/* Returns null when the stack is empty */
char *pop()
{
	char *top_val;
	struct stacknode *tmp;
	if (top == NULL) {
		printf("directory stack is empty!\n");
		return NULL;

	} else if (top == bottom) {
		top_val = (char *) malloc(strlen(top->data) + 1);
		handle_failure_stack(top_val);
		strcpy(top_val, top->data);
		free(top);
		top = NULL;
		bottom = NULL;

	} else {
		top_val = (char *) malloc(strlen(top->data) + 1);
		handle_failure_stack(top_val);
		strcpy(top_val, top->data);
		tmp = top;
		top = tmp->next;
		tmp->next = NULL;
		free(tmp);
	}

	return top_val;
}

const char *peek()
{
	char *top_val;
	if (top == NULL) {
		printf("directory stack is empty!\n");
		return NULL;

	} else {

		top_val = (char *) malloc(strlen(top->data) + 1);
		handle_failure_stack(top_val);
		strcpy(top_val, top->data);
	}

	return top_val;
}

void print_dir_stack()
{
	struct stacknode *tmp;
	if (top == NULL) {
		printf("directory stack is empty\n");

	} else {

		tmp = top;
		while (tmp != NULL) {
			printf("%s ", tmp->data);
			tmp = tmp->next;
		}

		printf("\n");
	}
}

int sizeofstack()
{
	struct stacknode *tmp;
	int size = 0;

	if (top == NULL) {
		return 0;

	} else {
		tmp = top;
		while (tmp != NULL) {
			size++;
			tmp = tmp->next;
		}
	}

	return size;
}

/* Displays failed messages */
void handle_failure_stack(void *ptr)
{
	if (ptr == NULL) {
		fprintf(stderr, "ERROR : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
}
