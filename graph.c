#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// struktura węzła

typedef struct node {

    int Number;
    struct node *Connections;

} node_t;

// struktura grafu

typedef struct graph {

    int Height;
    int Width;

    node_t *Nodes;      // [*NODE0, *NODE1, *NODE2, *NODE3, (...)]
    int *NodeIndexes;    // [3, 5, 6, (...)]

} graph_t;

// funkcja czytająca graf z pliku

graph_t *load_from_file (FILE *Stream) {
    return NULL;
}

// algorytm dzielący graf

graph_t **partition_graph (graph_t *Graph, int PartitionCount, double ErrorMargin) {
    return NULL;
}

// print graph

void print_graph (graph_t *map) {
    int NodeCount = 0;
    if (map == NULL){
        printf("Something is wrong with graph!");
    }

    for (int i = 0; i < map->Width; i++) {
        printf("+ -- ");
    }
    printf("+\n");

    for (int j = 0; j < map->Height; j++) {
        for (int k = 0; k < map->Width; k++){
            if (map->NodeIndexes[NodeCount] == j*(map->Width) + k){
                printf("|%3d ", map->Nodes[NodeCount].Number);
                NodeCount++; 
            } else {
                printf("|    ");
            }
            if (k == map->Width-1){
                printf("|\n");
                for (int l = 0; l < map->Width; l++) {
                    printf("+ -- ");
                }
                printf("+\n");
            }
        }
    }
}

// funkcje pomocnicze

// main

int main (int argc, char **argv) {
    node_t n0, n1, n2;

    graph_t *map = malloc(sizeof(graph_t));
    map->Height = 2;
    map->Width = 3;

    map->Nodes = malloc(3 * sizeof(node_t));
    map->Nodes[0] = n0;
    map->Nodes[1] = n1;
    map->Nodes[2] = n2;

    map->NodeIndexes = malloc(3 * sizeof(int));
    map->NodeIndexes[0] = 0;
    map->NodeIndexes[1] = 2;
    map->NodeIndexes[2] = 5;

    map->Nodes[0].Number = 0;
    map->Nodes[1].Number = 1;
    map->Nodes[2].Number = 2;

    map->Nodes[0].Connections = malloc(2 * sizeof(node_t));
    map->Nodes[1].Connections = malloc(1 * sizeof(node_t));
    map->Nodes[0].Connections[0] = map->Nodes[1];
    map->Nodes[0].Connections[1] = map->Nodes[2];
    map->Nodes[1].Connections[0] = map->Nodes[2];

    print_graph(map);

    printf("Connections:\n");
    for (int i = 0; i<2; i++){ // USTALIC DLUGOSCI!
        for (int j = 0; j<2; j++){
            printf("n%d -> n%d\n", map->Nodes[i].Number, map->Nodes[i].Connections[j]);
        }
    }

    free(map);
    
    return 0;
}
