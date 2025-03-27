#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// struktura węzła

typedef struct node {

    int Number;
    struct node **Connections;

} node_t;

// struktura grafu

typedef struct graph {

    int Height;
    int Width;

    node_t *Nodes;      // [*NODE0, *NODE1, *NODE2, *NODE3, (...)]
    int *NodeIndexes;    // [3, 5, 6, (...)]

} graph_t;

// funkcja czytająca graf z pliku

graph_t *new_graph (int Height, int Width, node_t *Nodes, int *NodeIndexes) {

    graph_t *NewGraph = malloc(sizeof(graph_t));

    NewGraph->Height = Height;
    NewGraph->Width = Width;
    NewGraph->Nodes = Nodes;
    NewGraph->NodeIndexes = NodeIndexes;

    return NewGraph;
}

node_t new_node (int Number, node_t **Connections) {
    node_t NewNode;
    
    NewNode.Number = Number;
    NewNode.Connections = Connections;

    return NewNode;
}

void free_graph (graph_t *Graph) {
    for (int i = 0; i < (int) sizeof(Graph->NodeIndexes)/sizeof(int); i++) {
        free(Graph->Nodes[i].Connections);
    }
    free(Graph->Nodes);
    free(Graph);
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

graph_t *load_from_file (FILE *Stream) {

    if (Stream == NULL) {
        fprintf(stderr, "[!] Error: Can't open file!\n");
        return NULL;
    }

    char Width[64];                 // max 64B
    char NodeIndexes[65536];        // max 64kB
    char FirstIndexes[65536];
    char Connections[65536];
    char ConnectionIndexes[65536];

    fgets(Width, 64, Stream);
    fgets(NodeIndexes, 65536, Stream);
    fgets(FirstIndexes, 65536, Stream);
    fgets(Connections, 65536, Stream);
    fgets(ConnectionIndexes, 65536, Stream);

    int GraphWidth = atoi(Width);

    int GraphNodeIndexes[65536];

    char *NodeIndex = NodeIndexes;
    int CurrentIndex = 0;
    while (*NodeIndex) {
        if (isdigit(*NodeIndex)) {
            long Value = strtol(NodeIndex, &NodeIndex, 10);
            GraphNodeIndexes[CurrentIndex] = Value;
            CurrentIndex++;
        } else {
            NodeIndex++;
        }
    }
    GraphNodeIndexes[CurrentIndex] = -1;

    int GraphFirstIndexes[65536];

    char *FirstIndex = FirstIndexes;
    CurrentIndex = 0;
    while (*FirstIndex) {
        if (isdigit(*FirstIndex)) {
            long Value = strtol(FirstIndex, &FirstIndex, 10);
            GraphFirstIndexes[CurrentIndex] = Value;
            CurrentIndex++;
        } else {
            FirstIndex++;
        }
    }
    GraphFirstIndexes[CurrentIndex] = -1;

    int GraphConnections[65536];

    char *Connection = Connections;
    CurrentIndex = 0;
    while (*Connection) {
        if (isdigit(*Connection)) {
            long Value = strtol(Connection, &Connection, 10);
            GraphConnections[CurrentIndex] = Value;
            CurrentIndex++;
        } else {
            Connection++;
        }
    }
    GraphConnections[CurrentIndex] = -1;

    int GraphConnectionIndexes[65536];

    char *ConnectionIndex = ConnectionIndexes;
    CurrentIndex = 0;
    while (*ConnectionIndex) {
        if (isdigit(*ConnectionIndex)) {
            long Value = strtol(ConnectionIndex, &ConnectionIndex, 10);
            GraphConnectionIndexes[CurrentIndex] = Value;
            CurrentIndex++;
        } else {
            ConnectionIndex++;
        }
    }
    GraphConnectionIndexes[CurrentIndex] = -1;
    
    node_t *NewGraphNodes = malloc(65536 * sizeof(node_t));
    int NewGraphNodeIndexes[65536];
    
    int CurrentNodeIndex = 0;
    int CurrentNodeNumber = 0;
    int NewGraphHeight = 0;
    while(GraphFirstIndexes[NewGraphHeight] != -1) {
        if (GraphFirstIndexes[NewGraphHeight + 1] == GraphFirstIndexes[NewGraphHeight]) {
            NewGraphHeight++;
            continue;
        }
        while(GraphFirstIndexes[NewGraphHeight + 1] != CurrentNodeNumber && GraphFirstIndexes[NewGraphHeight + 1] != -1) {
            NewGraphNodes[CurrentNodeNumber] = new_node(CurrentNodeNumber, NULL);
            NewGraphNodeIndexes[CurrentNodeNumber] = GraphNodeIndexes[CurrentNodeIndex] + (NewGraphHeight * GraphWidth);
            CurrentNodeIndex++;
            CurrentNodeNumber++;
        }
        NewGraphHeight++;
    }
    NewGraphHeight--;
    
    graph_t *NewGraph = new_graph(NewGraphHeight, GraphWidth, NewGraphNodes, NewGraphNodeIndexes);

    int GraphConnectionsIterator = 1;
    int GraphConnectionIndexesIterator = 0;

    while (GraphConnectionIndexes[GraphConnectionIndexesIterator] != -1) {
        int CurrentNodeToMakeConnectionsFor = GraphConnections[GraphConnectionIndexes[GraphConnectionIndexesIterator]];
        for (int i = 0; i < CurrentNodeNumber; i++) {
            if (NewGraph->Nodes[i].Number == CurrentNodeToMakeConnectionsFor) {
                NewGraph->Nodes[i].Connections = malloc (65536 * sizeof(node_t *));
                int NewGraphNodeConnectionsIterator = 0;
                while (GraphConnectionsIterator != GraphConnectionIndexes[GraphConnectionIndexesIterator + 1] /* && GraphConnectionIndexes[GraphConnectionIndexesIterator + 1] != -1 */) {
                    if (GraphConnections[GraphConnectionsIterator] == -1) {
                        break;
                    }
                    NewGraph->Nodes[i].Connections[NewGraphNodeConnectionsIterator] = &NewGraph->Nodes[GraphConnections[GraphConnectionsIterator]];
                    NewGraphNodeConnectionsIterator++;
                    GraphConnectionsIterator++;
                }
                NewGraph->Nodes[i].Connections[NewGraphNodeConnectionsIterator] = NULL;
                CurrentNodeToMakeConnectionsFor++;
                GraphConnectionsIterator++;
                break;
            }
        }
        GraphConnectionIndexesIterator++;
    }

    /* print_graph(NewGraph);

    for (int i = 0; i < 105; i++) {
        int j = 0;
        while(NewGraph->Nodes[i].Connections != NULL && NewGraph->Nodes[i].Connections[j] != NULL) {
            printf("%d -> %d\n", NewGraph->Nodes[i].Number, NewGraph->Nodes[i].Connections[j]->Number);
            j++;
        }
    } 
        
    free_graph(NewGraph); */


    return NewGraph;
}

// algorytm dzielący graf

graph_t **partition_graph (graph_t *Graph, int PartitionCount, double ErrorMargin) {
    return NULL;
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
    //map->Nodes[0].Connections[0] = map->Nodes[1];
    //map->Nodes[0].Connections[1] = map->Nodes[2];
    //map->Nodes[1].Connections[0] = map->Nodes[2];

    print_graph(map);

    printf("Connections:\n");
    for (int i = 0; i<2; i++){ // USTALIC DLUGOSCI!
        for (int j = 0; j<2; j++){
            printf("n%d -> n%d\n", map->Nodes[i].Number, map->Nodes[i].Connections[j]);
        }
    }

    free(map);

    FILE *Stream = fopen("graf.cssrg", "r+");
    load_from_file(Stream);
    
    fclose(Stream);
    return 0;
}
