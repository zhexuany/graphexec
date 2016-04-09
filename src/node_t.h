#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/* for status variable:*/
#define INELIGIBLE 0
#define READY 1
#define RUNNING 2
#define FINISHED 3

/* define a string type*/
typedef char* string;

typedef struct node {
  int id; // corresponds to line number in graph text file
  char prog[1024]; // prog + arguments
  char input[1024]; // filename
  char output[1024]; // filename
  int children[10]; // children IDs
  int num_children; // how many children this node has
  int status;
  int num_parent; // how many paretns do the child have
  int parent_done; // how many parents of the chiid are finished
  pid_t pid; // track it when it's running
  int color; // the color of each node
} node_t;
/*create node according to the number of lines in file*/
int creatGraph(FILE* fp, node_t* node);
/*check and update the status of node*/
int runGraph(node_t* node, int num_node);
int nodeStatus(node_t* node);
/*execute node*/
int execNode(node_t node);
/*Use DFS for finding cycle in graph*/
void detectCycle(node_t* node, int num_node);
void visit(node_t* node, int i);
