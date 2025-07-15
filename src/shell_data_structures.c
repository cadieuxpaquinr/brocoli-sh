#include "shell_data_structures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void queue_initialize(queue *q)
{
   q->first = NULL;
   q->last = NULL;
   q->size = 0;
}

int queue_is_empty(const queue *q) { return q->first == NULL || q->size == 0; }

void queue_push(queue *q, char *src_token)
{
   struct queue_node *node = malloc(sizeof(struct queue_node));
   node->token = strdup(src_token);
   node->prev = q->last;
   node->next = NULL;
   if (queue_is_empty(q))
      q->first = node;
   else
      q->last->next = node;
   q->last = node;
   q->size++;
}

char *queue_pop(queue *q)
{
   if (!queue_is_empty(q))
   {
      q->size--;
      if (q->first == q->last)
         q->last = NULL;
      char *token = NULL;
      strcpy(token, q->first->token);
      struct queue_node *node = q->first;
      q->first = node->next;
      free(node);
      return token;
   }
   else
   {
      fprintf(stderr, "Cannot pop from empty queue\n");
      exit(1);
   }
}

void queue_print(const queue *q)
{
   fprintf(stdout, "[ ");
   struct queue_node *node = q->first;
   while (node != NULL)
   {
      fprintf(stdout, "%s", node->token);
      node = node->next;
   }
   fprintf(stdout, " ]\n");
}

void queue_delete(queue *q)
{
   while (!queue_is_empty(q))
      queue_pop(q);
}