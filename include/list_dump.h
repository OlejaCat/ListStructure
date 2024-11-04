#ifndef LIST_DUMP_H
#define LIST_DUMP_H

#include "list.h"

typedef enum ListDumpError
{
    ListDumpError_ERROR   = -1,
    ListDumpError_SUCCESS =  0,
} ListDumpError;

#define LIST_DUMP_FILE_PATH "list_dump.htm"
#define LIST_DUMP_PNG_FILE  "graph.png"

ListDumpError dumpList_(List*       list,
                        const char* variable_name);

#define dumpList(list_) dumpList_(list_, #list_)

#endif // LIST_DUMP_H
