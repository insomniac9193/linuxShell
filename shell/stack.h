
struct stacknode {
	char *data;
	struct stacknode *next;
};

/* Initializes the stack */
void initstack();

/* Pushes the element on top of stack */
void push(const char *);

/* Pops the top most element on stack and returns its value */
char *pop();

/* Returns the top most element on the stack without removing it*/
const char *peek();

/* Returns the size of stack */
int sizeofstack();

/* print stack contents */
void print_dir_stack();

/* Displays error messages in case of failures */
void handle_failure_stack(void *);


