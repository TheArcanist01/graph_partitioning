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

void print_graph (graph_t *Graph) {
    int NodeCount = 0;
    if (Graph == NULL){
        printf("Something is wrong with graph!");
    }

    for (int i = 0; i < Graph->Width; i++) {
        printf("+ -- ");
    }
    printf("+\n");

    for (int j = 0; j < Graph->Height; j++) { 
        for (int k = 0; k < Graph->Width; k++){
            if (Graph->NodeIndexes[NodeCount] == j*(Graph->Width) + k){
                printf("|%3d ", Graph->Nodes[NodeCount].Number);
                NodeCount++; 
            } else {
                printf("|    ");
            }
            if (k == Graph->Width-1){
                printf("|\n");
                for (int l = 0; l < Graph->Width; l++) {
                    printf("+ -- ");
                }
                printf("+\n");
            }
        }
    }
    for (int i = 0; i < 105; i++) {
        int j = 0;
        while(Graph->Nodes[i].Connections != NULL && Graph->Nodes[i].Connections[j] != NULL) {
            printf("%d -> %d\n", Graph->Nodes[i].Number, Graph->Nodes[i].Connections[j]->Number);
            j++;
        }
    } 
}

// funkcja czytająca graf z pliku

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
    return NewGraph;
}

// algorytm dzielący graf

graph_t **partition_graph (graph_t *Graph, int PartitionCount, double ErrorMargin) {
    return NULL;
}

// funkcje pomocnicze

// funkcja zwraca dlugosc tablicy NodeIndexes
int how_many_nodes(graph_t *Graph){
    int size = 0;
    while (Graph->NodeIndexes[size] != 0){
        size++;
    }
    return size;
}

// zapis do pliku tekstowego
void write_to_txt(graph_t *Graph){//, char *name){
    FILE *out = fopen("res.txt", "w");
    if (out == NULL){
        printf("Can't access the file!\n");
        return;
    }
    //fprintf(out, "%s\n", name); // DOPISAC POZOSTALE ARGS
    int Index = 0;
    for (int i=0; i<Graph->Height; i++){
        fprintf(out, "[");
        for (int j=0; j<Graph->Width; j++){
            if (Graph->NodeIndexes[Index] == (i*Graph->Width)+j){
                fprintf(out, "1. ");
                Index++;
            } else {
                fprintf(out, "0. ");
            }
        }
        fprintf(out, "]\n");
    }
    for (int i = 0; i < 105; i++) {
        int j = 0;
        while(Graph->Nodes[i].Connections != NULL && Graph->Nodes[i].Connections[j] != NULL) {
            fprintf(out, "%d - %d\n", Graph->Nodes[i].Number, Graph->Nodes[i].Connections[j]->Number);
            j++;
        }
    }
    fclose(out);
    printf("zapisano txt\n");
}

// dzieki temu kod jest bardziej przejrzysty
int d (graph_t *Graph, int i, int j){
    if (Graph->Nodes[i].Connections != NULL && Graph->Nodes[i].Connections[j] != NULL){
        return 1;
    } else {
        return 0;
    }
}

//zapis do pliku binarnego 
void write_to_bin(graph_t *Graph){
    int Index = 0;
    char minus = '-';
    char one = '1';
    char zero = '0';
    char dot = '.';
    char n = '\n';
    //char spr;

    FILE *out = fopen("res.bin", "wb+");
    if (out == NULL){
        printf("Can't open the file!");
        return;
    }
    for (int i = 0; i<Graph->Height; i++){
        for (int j = 0; j<Graph->Width; j++){
            if (Graph->NodeIndexes[Index] == (i*Graph->Width)+j){
                fwrite(&one, sizeof(int), 1, out);
                Index++;
            } else {
                fwrite(&zero, sizeof(int), 1, out);
            }
            fwrite(&dot, sizeof(int), 1, out);
        }
        fwrite(&n, sizeof(int), 1, out);
    }

    char tmp;
    for (int i = 0; i < 105; i++) {
        int j = 0;
        while(d(Graph, i, j)) {
            tmp = (char)Graph->Nodes[i].Number;
            fwrite(&tmp, sizeof(int), 1, out);
            fwrite(&minus, sizeof(int), 1, out);
            tmp = (char)Graph->Nodes[i].Connections[j]->Number;
            fwrite(&tmp, sizeof(int), 1, out);
            fwrite(&n, sizeof(int), 1, out);
            j++;
        }
    }
    /*fseek(out, 0, SEEK_SET);
    for (int i = 0; i<Graph->Height; i++){
        for (int j = 0; j<Graph->Width; j++){
            fread(&spr, sizeof(int), 1, out);
            printf("%c", spr);
            fread(&spr, sizeof(int), 1, out);
            printf("%c", spr);
        }
        fread(&spr, sizeof(int), 1, out);
        printf("%c", spr);
    }

    for (int i = 0; i < 105; i++) {
        int j = 0;
        while(d(Graph, i, j)) {
            fread(&spr, sizeof(int), 1, out);
            printf("%d", spr);
            fread(&spr, sizeof(int), 1, out);
            printf("%c", spr);
            fread(&spr, sizeof(int), 1, out);
            printf("%d", spr);
            fread(&spr, sizeof(int), 1, out);
            printf("%c", spr);
            j++;
        }
    }*/
    fclose(out);
    printf("zapisano bin");
}


// main

int main (int argc, char **argv) {
    //FILE *Stream = fopen("graf.csrrg", "r+");
    FILE *Stream = fopen("przyklad.csrrg", "r+");

    graph_t *NewGraph = malloc(sizeof(graph_t));
    NewGraph = load_from_file(Stream);  
    fclose(Stream);
    print_graph(NewGraph);
    
    write_to_txt(NewGraph);

    write_to_bin(NewGraph);

    free_graph(NewGraph);
    return 0;
}
