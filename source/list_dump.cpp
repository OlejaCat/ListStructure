#include "list_dump.h"

#include <stdio.h>
#include <assert.h>


// static --------------------------------------------------------------------------------------------------------------


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

#define TEMP_DOT_FILE_PATH "temp_list_graph.dot"

static ListDumpError dumpGraphviz(List* list);


// public --------------------------------------------------------------------------------------------------------------


ListDumpError dumpList_(List*       list,
                        const char* variable_name)
{
    assert(list          != NULL);
    assert(variable_name != NULL);

    FILE* output_file = fopen(LIST_DUMP_FILE_PATH, "w");
    if (!output_file)
    {
        return ListDumpError_ERROR;
    }

    fprintf(output_file, "<pre>\n");
    fprintf(output_file, "List variable \"%s\"\nCreated in %s:%d, function: %s\n",
                         variable_name,
                         list->file_name,
                         list->line,
                         list->function_name);

    fprintf(output_file, "\"%s\" pointer [%p]\n", variable_name, list);
    fprintf(output_file, "\tnode_array  [%p]\n" , list->node_array);
    fprintf(output_file, "\tfree_node = %lu\n"  , list->free_node);
    fprintf(output_file, "\tcapacity  = %lu\n"  , list->capacity);

    fprintf(output_file, "</pre>\n");

    dumpGraphviz(list);

    fprintf(output_file, "<img src=\"" LIST_DUMP_PNG_FILE "\" />");

    fclose(output_file);

    return ListDumpError_SUCCESS; // лист думп эрор сукесекес
}


// static --------------------------------------------------------------------------------------------------------------


static ListDumpError dumpGraphviz(List* list)
{
    FILE* output_file = fopen(TEMP_DOT_FILE_PATH, "w");
    if (!output_file)
    {
        return ListDumpError_ERROR;
    }

    fprintf(output_file, "digraph G\n{");
    fprintf(output_file, "    rankdir=LR;\n");
    fprintf(output_file, "    bgcolor=\"gray20\";\n");          // Устанавливаем фон графа в серый
    fprintf(output_file, "    edge [color=white];\n");         // Устанавливаем цвет ребер на белый
    fprintf(output_file, "    node [shape=record, style=filled, fillcolor=\"#4b4b4b\", fontcolor=white];\n\n");

    fprintf(output_file,
            "node0 [label=\"ip: 0 | data: %d | next: %lu | prev: %lu\"];\n",
            list->node_array[0].data,
            list->node_array[0].next,
            list->node_array[0].prev);

    size_t size_of_data = 1;
    size_t current_index = 0;
    while (list->node_array[current_index].next != 0)
    {
        current_index = list->node_array[current_index].next;

        fprintf(output_file,
                "node%lu [label=\"ip: %lu | data: %d | next: %lu | prev: %lu\"];\n",
                size_of_data,
                current_index,
                list->node_array[current_index].data,
                list->node_array[current_index].next,
                list->node_array[current_index].prev);

        size_of_data++;
    }

    fprintf(output_file, "head [shape=hexagon, label=\"HEAD\"];\n");
    fprintf(output_file, "tail [shape=hexagon, label=\"TALE\"];\n");

    for (size_t index = 0; index < size_of_data; index++)
    {
        size_t first  = index;
        size_t second = (index + 1) % size_of_data;

        fprintf(output_file, "node%lu -> node%lu [color=red];\n",  first,  second);
        fprintf(output_file, "node%lu -> node%lu [color=lightblue];\n", second, first);
    }

    if (size_of_data > 1)
    {
        fprintf(output_file, "head -> node1 [color=orange];\n");
        fprintf(output_file, "tail -> node%lu [color=orange];\n", size_of_data - 1);
    }

    size_t size_of_free_data = size_of_data;
    size_t current_free_index = list->free_node;
    while (current_free_index <= list->capacity)
    {
        fprintf(output_file,
                "node%lu [label=\"ip: %lu | data: - | next: %lu | prev: -\"];\n",
                size_of_data,
                current_free_index,
                list->node_array[current_free_index].next);

        current_free_index = list->node_array[current_free_index].next;
        size_of_data++;
    }

    fprintf(output_file, "free [shape=hexagon, label=\"FREE\"];\n");

    if (size_of_free_data < size_of_data)
    {
        fprintf(output_file, "free -> node%lu [color=orange]", size_of_free_data);
    }

    for (size_t index = size_of_free_data; index < size_of_data - 1; index++)
    {
        fprintf(output_file, "node%lu -> node%lu [color=green];\n", index, index + 1);
    }

    fprintf(output_file, "}\n");

    fclose(output_file);

    const char* command = "dot -Tpng " TEMP_DOT_FILE_PATH " -o " LIST_DUMP_PNG_FILE;

    system(command);
    system("rm -f " TEMP_DOT_FILE_PATH);

    return ListDumpError_SUCCESS;
}
