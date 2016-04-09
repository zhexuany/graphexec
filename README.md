## Project Description ##
This is project is about the practice of `fork()` and `execp()`. We were required to
read a file and parse the contents. 
According to the contents, we need fork a lot processes and then overwrite them for 
execute some Unix commands that specified in files.
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
## How to compile and test ##
Compile is relative easy, you can clone this repository first and enter make
from the terminal window. 
For the testing, it is also easy. If you successfully compiled the program, you
will see a graphexec file that you can execute. By entering
./graphexec [file name]
you can test the correctness of the program. 
