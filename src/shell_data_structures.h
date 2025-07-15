#ifndef BRO_SHELL_STRUCTURES_H
#define BRO_SHELL_STRUCTURES_H

// Types
// -----

struct queue_node
{
   char *token;
   struct queue_node *prev; // Previous node
   struct queue_node *next; // Next node
};

typedef struct
{
   struct queue_node *first; // Pointer to first node
   struct queue_node *last;  // Pointer to last node
   int size;
} queue;

// Interface
// ---------

/**
 * Initializes a queue
 *
 * Note: do not forget to call `queue_delete` when the queue is not needed
 * anymore.
 *
 * @return  The queue
 */
void queue_initialize(queue *q);

/**
 * Returns true if the queue is empty
 *
 * @param q  The queue to check
 * @return   true if and only if the queue is empty
 */
int queue_is_empty(const queue *q);

/**
 * Pushes an element at the end of the queue
 *
 * @param q      The queue
 * @param value  The element to push
 */
void queue_push(queue *q, char *src_token);

/**
 * Pops the first element of a queue
 *
 * The element is removed from the queue and returned.
 *
 * @param q  The queue
 * @return   The value of the top of the queue
 */
char *queue_pop(queue *q);

/**
 * Prints the queue to stdout
 *
 * @param q  The queue
 */
void queue_print(const queue *q);

/**
 * Deletes a queue
 *
 * @param q  The queue to delete
 */
void queue_delete(queue *q);

#endif