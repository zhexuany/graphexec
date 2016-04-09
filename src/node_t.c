#include "node_t.h"
#include "makeargv.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>


// define global variables
#define COMMAND_DELIMITER ":\n"
#define CHILD_DEILMITER " "
#define BUFFER_SZIE 1024
#define MAX_NODES  50
#define O_FLAGS O_WRONLY | O_CREAT | O_TRUNC
#define I_FLAGS O_RDONLY
#define PERM 0644
// defind color for DFS and detect cycle
#define WHITE 4
#define GRAY 5
#define BLACK 6


int execNode(node_t node) {
  string* prog;
  if (makeargv(node.prog, CHILD_DEILMITER, &prog) == 0) {
    perror("No Process name");
    return -1;
  }
  int f_in, f_out;

  if ((f_in = open(node.input, I_FLAGS)) <= 0) {
    perror("Error opening input file in child after fork! Exiting.");
    exit(0);
  } else {
    dup2(f_in, STDIN_FILENO);
    close(f_in);
  }

  if ((f_out = open(node.output, O_FLAGS, PERM)) <= 0) {
    perror("Error opening output file in child after fork! Exiting.");
    exit(0);
  } else {
    dup2(f_out, STDOUT_FILENO);
    close(f_out);
  }

  // execute node
  execvp(prog[0], prog);
  perror("Failed to execute");

  return 0;

}

int runGraph(node_t* node, int num_node) {
  int done, i, j;
  done = 0;
  while (done < num_node) {
    for (i = 0; i < num_node; i++) {
      //   printf("node 2 %d\n", node[i].num_parent);
      if (nodeStatus(&node[i]) != 0) {
          done++;
          // update the number of parents of the chils have completed.
          for (j = 0; j < node[i].num_children; ++j) {
            node[node[i].children[j]].parent_done++;
          }
        }
      }
    }
  return 0;
}


int nodeStatus(node_t* node) {
      if (node -> status == INELIGIBLE) {
        // updated the node's status and make it ready to execute
        //printf("node 2 %d\n", node -> num_parent);
        if (node -> parent_done == node -> num_parent) {
          node -> status = READY;
        }
      }

      if (node -> status == READY) {
        // if the node is ready to execute, call fork
        printf("Num%d node is reday\n", node -> id);
        pid_t childpid = fork();
        if (childpid == -1) {
          perror("Failed to fork");
          exit(1);
        }

        if (childpid == 0) {
          // execute the node
          execNode(*node);
        } else {
          printf("Num%d node is runing\n", node -> id);
          node -> status = RUNNING;
          node -> pid = childpid;
        }
      }


      if (node -> status == RUNNING) {
        int waitreturn = waitpid(node -> pid, & (node -> status), WNOHANG);
        if (waitreturn == -1) {
          perror("Failed to get the node's status");
          exit(1);
        } else if (waitreturn != 0) {
          printf("Num%d node is completed\n", node -> id);
          node -> status = FINISHED;
          return waitreturn;
        }
      }
  return 0;
}

int creatGraph(FILE* fp, node_t* node){
  int num_token, num_line, i, j;
  char line_buffer[BUFFER_SZIE];
  string* command;
  string* childlist;
  num_line = 0;
  while ((fgets(line_buffer, sizeof line_buffer, fp))!=NULL) {
    /*Replace newline with null*/
    if (line_buffer[strlen(line_buffer) - 1] == '\n'){
      line_buffer[strlen(line_buffer) - 1] = 0;
    }

    // skip blank line or with witespcae
    if (strcmp(line_buffer, "\0") != 0) {
      num_token = makeargv(line_buffer, COMMAND_DELIMITER, &command);

    if (num_token != 4) {
      fprintf(stderr, "Illegal contexts from file\n");
      return -1;
    }

    node -> id = num_line;
    node -> status = INELIGIBLE;
    strcpy(node -> prog, command[0]);

    // set every node is not travelled
    node -> color = WHITE;

    if (!strcmp(command[1], "none")) {
      node -> num_children = 0;
    } else {

      node -> num_children = makeargv(command[1], CHILD_DEILMITER, &childlist);
      i = 0;
      j = 0;
      while(i < node -> num_children) {
        node -> children[i] = atoi(childlist[i]);
        i++;
      }
      freemakeargv(childlist);
    }

    /*copy input and output file name to correspoding node*/
    strcpy(node -> input, command[2]);
    strcpy(node -> output, command[3]);
    num_line++;

    node ++;
    } else {
      continue;
  }
  }



  freemakeargv(command);
  node -= num_line;

  // updated the number of parents of the child
  for (i = 0; i < num_line; i++) {

    for (j = 0; j < node[i].num_children; j++) {
      node[node[i].children[j]].num_parent += 1;
      // printf("node 2 %d\n", node[i].num_parent);
    }
  }

  return num_line;
}

void visit(node_t* node, int i) {
  node[i].color = GRAY;
  int j;
  for (j = 0; j < node[i].num_children; j++) {
    if (node[node[i].children[j]].color == GRAY && i > node[i].children[j] ) {
      fprintf(stderr, "cycle found in graph\n");
      exit(0);
    }

    if (node[node[i].children[j]].color == WHITE) {
      return visit(node, node[i].children[j]);
    }
  }
  node[i].color = BLACK; // balck this node; it is finished.

}

void detectCycle(node_t* node, int num_node) {
  int i = 0;
  while (i < num_node) {
    if (node[i].color == WHITE) {
      visit(node, i);
    }
    i++;
  }
}
