#include <stdlib.h>
#include "linked_list.h"

// Creates and returns a new list
list_t *list_create()
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
    return list;
}

// Destroys a list
void list_destroy(list_t *list)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    // free all nodes
    while (list->count > 0)
    {
        list_remove(list, list->head);
    }

    free(list);
}

// Returns head of the list
list_node_t *list_head(list_t *list)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return list->head;
}

// Returns tail of the list
list_node_t *list_tail(list_t *list)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return list->tail;
}

// Returns next element in the list
list_node_t *list_next(list_node_t *node)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return node->next;
}

// Returns prev element in the list
list_node_t *list_prev(list_node_t *node)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return node->prev;
}

// Returns end of the list marker
list_node_t *list_end(list_t *list)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return NULL;
}

// Returns data in the given list node
void *list_data(list_node_t *node)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return node->data;
}

// Returns the number of elements in the list
size_t list_count(list_t *list)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    return list->count;
}

// Finds the first node in the list with the given data
// Returns NULL if data could not be found
list_node_t *list_find(list_t *list, void *data)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    size_t count = list->count;
    list_node_t *current = list->head;
    for (int i = 0; i < count; i++)
    {
        // if data is not found, move to next node
        if (current->data != data)
        {
            current = current->next;
        }
        // if data is found, return node
        else
        {
            return current;
        }
    }
    // if data is not found, return NULL
    return NULL;
}

// Inserts a new node in the list with the given data
// Returns new node inserted
list_node_t *list_insert(list_t *list, void *data)
{
    /* IMPLEMENT THIS IF YOU WANT TO USE LINKED LISTS */
    list_node_t *new_node = malloc(sizeof(list_node_t));
    new_node->data = data;
    new_node->next = NULL;

    // list is empty
    if (list->head == NULL)
    {
        list->head = new_node;
        new_node->prev = NULL;
        list->tail = new_node;
    }

    // list is not empty, append it to tail
    else
    {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->count++;
    return new_node;
}

// Removes a node from the list and frees the node resources
void list_remove(list_t *list, list_node_t *node)
{
    // invalid input
    if (list == NULL || node == NULL)
    {
        return;
    }

    // if node is only node in list
    if (node->next == NULL && node->prev == NULL)
    {
        list->head = NULL;
        list->tail = NULL;
        list->count--;
        free(node);
    }

    // if node is in middle of list
    else if (node->next != NULL && node->prev != NULL)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        list->count--;
        free(node);
    }
    // if node is head
    else if (node->next != NULL && node->prev == NULL)
    {
        node->next->prev = NULL;
        list->head = node->next;
        list->count--;
        free(node);
    }
    // if node is tail
    else if (node->next == NULL && node->prev != NULL)
    {
        node->prev->next = NULL;
        list->tail = node->prev;
        list->count--;
        free(node);
    }
}
