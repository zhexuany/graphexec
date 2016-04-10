## Project Description ##
This is project is about the practice of `fork()` and `execp()`. We were required to
read a file and parse the contents. 
According to the contents, we need fork a lot processes and then overwrite them for 
execute some Unix commands that specified in files.

The following snippets is copied from `node_t.c`. `creatGraph()` is the key function
of this project. It can read contents from a file, create a grapg of nodes. 
```java
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
```
The following snippets is copied from `node_t.c` file and is the key function that
execute the compand specified by the file contents.
```java
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
```

It is highly possihle that the graph wanted the program to create has cycle. In such
case, we should be able to detect it and report it because graph of nodes will break
the realtionship between parent nodes and child nodes. For saftey and correctness, we
can't let our program create a graph having a cycle. 

## How to compile and test ##
Compile is relative easy, you can clone this repository first and enter make
from the terminal window. 
For the testing, it is also easy. If you successfully compiled the program, you
will see a graphexec file that you can execute. By entering
./graphexec [file name]
you can test the correctness of the program. 
