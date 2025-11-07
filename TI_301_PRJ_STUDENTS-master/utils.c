#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include <hasse.h>

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}

void writeGraphFile(t_adjlist adj)
{
    FILE *file = fopen("example_valid_step3.txt", "wt");
    if (file == NULL)
    {
        printf("Error creating file.\n");
        exit(1);
    }

    fprintf(file, "---\n");
    fprintf(file, "config:\n");
    fprintf(file, "   layout: elk\n");
    fprintf(file, "   theme: neo\n");
    fprintf(file, "   look: neo\n");
    fprintf(file, "---\n\n");

    fprintf(file, "flowchart LR\n");

    for (int i = 0; i < adj.size; i++)
    {
        fprintf(file, "%s((%d))\n", getID(i + 1), i + 1);
    }

    for (int i = 0; i < adj.size; i++)
    {
        t_cell *current = adj.list[i].head;
        while (current != NULL)
        {
            fprintf(file, "%s -->|%.2f|%s\n", getID(i + 1), current->probability, getID(current->arrival));
            current = current->next;
        }
    }

    fclose(file);
    printf("file example_valid_step3.txt created\n");
}
