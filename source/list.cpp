#include "list.h"

#include <string.h>
#include <stdbool.h>
#include <assert.h>


// static --------------------------------------------------------------------------------------------------------------


typedef int list_type;

typedef struct Node
{
    list_type data;
    size_t    next;
    size_t    prev;
    bool      in_use;
} Node;

typedef struct List
{
    Node*       node_array;

    size_t      free_node;
    size_t      capacity;

    const char* file_name;
    int         line;
    const char* function_name;
} List;

static size_t SCALE_FACTOR = 2;

static ListOperationError listResize(List* list);


// public --------------------------------------------------------------------------------------------------------------


// -------------------------------  INDEX OPERATIONS -------------------------------


size_t getNextIndex(List* list, size_t index)
{
    assert(list != NULL);

    return list->node_array[index].next;
}


size_t getPreviousIndex(List* list, size_t index)
{
    assert(list != NULL);

    return list->node_array[index].prev;
}


// -------------------------------  CTOR AND DTOR  -------------------------------


List* listCtor_(size_t      capacity,
                const char* file,
                int         line,
                const char* function)
{
    List* list = (List*)calloc(1, sizeof(List));

    list->capacity = capacity;

    list->node_array = (Node*)calloc(capacity + 1, sizeof(Node));
    if (list->node_array == NULL)
    {
        return NULL;
    }

    list->node_array[0] = {
        .data   = 0,
        .next   = 0,
        .prev   = 0,
        .in_use = true,
    };

    for (size_t index = 1; index <= capacity; index++)
    {
        list->node_array[index] = {
            .data   = 0,
            .next   = index++,
            .prev   = 0,
            .in_use = false,
        };
    }

    list->free_node = 1;

    list->file_name     = file;
    list->line          = line;
    list->function_name = function;

    return list;
}


ListOperationError listDtor(List* list)
{
    if (list == NULL)
    {
        return ListOperationError_ERROR;
    }

    free(list->node_array);
    free(list);

    return ListOperationError_SUCCESS;
}


// -------------------------------  INSERT OPERATIONS  -------------------------------


ListOperationError listInsert(List* list, size_t index, list_type element)
{
    assert(list != NULL);

    if (!list->node_array[index].in_use
     || index > list->capacity)
    {
        return ListOperationError_ERROR;
    }

    size_t real_index = list->free_node;
    if (real_index > list->capacity)
    {
        listResize(list);
    }

    list->free_node = list->node_array[real_index].next;

    list->node_array[real_index] = {
        .data   = element,
        .next   = list->node_array[index].next,
        .prev   = index,
        .in_use = true,
    };

    list->node_array[list->node_array[index].next].prev = real_index;
    list->node_array[index].next = real_index;

    return ListOperationError_SUCCESS;
}


ListOperationError listInsertTail(List* list, list_type element)
{
    assert(list != NULL);

    listInsert(list, 0, element);

    return ListOperationError_SUCCESS;
}


ListOperationError listInsertHead(List* list, list_type element)
{
    assert(list != NULL);

    listInsert(list, getPreviousIndex(list, 0), element);

    return ListOperationError_SUCCESS;
}


// ------------------------------- DELETE ELEMENTS -------------------------------


ListOperationError listDeleteElement(List* list, size_t index)
{
    assert(list != NULL);

    if (!list->node_array[index].in_use)
    {
        return ListOperationError_ERROR;
    }

    size_t prev = list->node_array[index].prev;
    size_t next = list->node_array[index].next;

    list->node_array[prev].next = next;
    list->node_array[next].prev = prev;

    list->node_array[index] = {
        .data   = 0,
        .next   = list->free_node,
        .prev   = 0,
        .in_use = false,
    };

    list->free_node = index;

    return ListOperationError_SUCCESS;
}


ListOperationError listDeleteHead(List* list)
{
    assert(list != NULL);

    listDeleteElement(list, getNextIndex(list, 0));

    return ListOperationError_SUCCESS;
}


ListOperationError listDeleteTail(List* list)
{
    assert(list != NULL);

    listDeleteElement(list, getPreviousIndex(list, 0));

    return ListOperationError_SUCCESS;
}


// static --------------------------------------------------------------------------------------------------------------


static ListOperationError listResize(List* list)
{
    assert(list != NULL);

    list->capacity *= SCALE_FACTOR;
    list->node_array = (Node*)realloc(list->node_array, list->capacity * sizeof(Node));
    if (list->node_array == NULL)
    {
        return ListOperationError_ERROR;
    }

    for (size_t index = list->free_node; index <= list->capacity; index++)
    {
        list->node_array[index] = {
            .data   = 0,
            .next   = index++,
            .prev   = 0,
            .in_use = false,
        };
    }

    return ListOperationError_SUCCESS;
}
