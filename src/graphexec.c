#include <stdio.h>
#include <stdlib.h>
#include "node_t.h"

// define global varibal
#define MAX_NODES  50

int main(int argc, char *argv[]) {
  string filename;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [Filename.txt]\n", argv[0]);
    exit(1);
  } else {
    if ((filename = (string)malloc(sizeof(string))) != NULL) {
      filename = argv[1];
    }else {
      perror("Error on allocated memory");
      exit(1);
      }
    }

  FILE* fp = fopen(filename, "r");

  if (!fp) {
    perror("File opending failed");
    return EXIT_FAILURE;
  }
  // Allocate memeory for nodes
  node_t* node;
  if ((node = (node_t*)calloc( sizeof (node_t), MAX_NODES)) == NULL) {
    perror("Error on allocated memory for nodes");
    exit(1);
  }

  /* start to create a graph */
  int counter_node;
  /*if number of nodes exceeds 50, throw a error*/
  if ((counter_node = creatGraph(fp, node)) > 50) {
    perror("Too many nodes");
    return -1;
  }

  fclose(fp);
  detectCycle(node, counter_node);
  runGraph(node, counter_node);
  free(node);

  return 0;
  }
