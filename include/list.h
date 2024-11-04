#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef int list_type;

typedef enum ListOperationError
{
    ListOperationError_ERROR               = -1,
    ListOperationError_SUCCESS             =  0,
    ListOperationError_ERROR_CTOR          =  1,
    ListOperationError_ERROR_INSERT_BEFORE =  2,
    ListOperationError_ERROR_INSERT_AFTER  =  3,
    ListOperationError_ERROR_POP_HEAD      =  4,
    ListOperationError_ERROR_POP_TAIL      =  5,
    ListOperationError_ERROR_DEL           =  6,
} ListOperationError;

typedef struct List List;

size_t getNextIndex(List* list, size_t index);
size_t getPreviousIndex(List* list, size_t index);

List* listCtor(size_t capacity);
ListOperationError listInsert(List* list, size_t index, list_type element);
ListOperationError listInsertTail(List* list, list_type element);
ListOperationError listInsertHead(List* list, list_type element);
ListOperationError listDtor(List* list);

ListOperationError listDeleteElement(List* list, size_t index);
ListOperationError listDeleteTail(List* list);
ListOperationError listDeleteHead(List* list);

#endif // LIST_H
