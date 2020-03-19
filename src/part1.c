#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asteroid_map.h"

#define WIDTH 25
#define HEIGHT 6
#define MAX_LAYERS 100

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char row_buffer[1024];
    
    memset(&row_buffer, '\0', sizeof(row_buffer));
    
    asteroid_map * baseMap = malloc(sizeof(asteroid_map));
    memset(baseMap, '\0', sizeof(asteroid_map));
    
    asteroid_map * bestMap = NULL;
    int bestNumVisible=0;
    
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        exit(1);
    }
    
    fprintf(stderr, "Opening file %s\n", argv[1]);
    
    infile=fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }
    
    while (fgets(row_buffer, sizeof(row_buffer), infile))
    {
        char * eol = strchr(row_buffer, '\r');
        if (eol)
        {
            *eol='\0';
        }
        eol = strchr(row_buffer, '\n');
        if (eol)
        {
            *eol='\0';
        }

        int len = strlen(row_buffer);
        char * new_row = malloc(sizeof(char)*len);
        memcpy(new_row, row_buffer, sizeof(char)*len);
        add_row(baseMap, len, new_row);
    }
    
    fclose(infile);
    
    print_map(baseMap);
    
    for (int i=0; i<baseMap->rows; i++)
    {
        for (int j=0; j<baseMap->cols; j++)
        {
            if (is_asteroid(baseMap, j, i))
            {
                asteroid_map * workingMap = copy_map(baseMap);
                processMapWithSource(workingMap, j, i);
                int count = count_visible(workingMap);
                printf("Map with source %d,%d has %d visible asteroids\n", j, i, count);
                print_map(workingMap);
                if (count > bestNumVisible)
                {
                    printf("****Setting to best map\n");
                    if (bestMap != NULL)
                        delete_map(bestMap);
                    bestMap=copy_map(workingMap);
                    bestNumVisible=count;
                }
                delete_map(workingMap);
            }
        }
    }
    
    printf("***The best map has %d visible\n", bestNumVisible);
    print_map(bestMap);
    delete_map(bestMap);
    
    delete_map(baseMap);
    
    return 0;
}
