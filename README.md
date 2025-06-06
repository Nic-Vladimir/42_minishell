<h2> 1)Define your data structure: </h2>

```C
typedef struct s_command_data
{
    char **args;
    t_env *env;
    int input_fd;
    int output_fd;
} t_command_data;
```

<h2> 2) Define your function: </h2>

```C
static int command_child_func(void *data)
{
    t_command_data *cmd = (t_command_data *)data;
    // Execute command logic
    return execve(cmd->args[0], cmd->args, env_to_array(cmd->env));
}
```

<h2> 3)Use the generic executor: </h2>

```C
int execute_command(char **args, t_env *env)
{
    t_command_data cmd_data = {args, env, fd_in, fd_out};
    t_child_data child_data = {&cmd_data, command_child_func, NULL, CHILD_SIG_DEFAULT};
    
    return execute_in_child(&child_data);
}
```
Benefits:

Reusable: One function handles all child process creation
Flexible: Different signal behaviors, cleanup functions
Clean: Separates child logic from process management
Maintainable: Changes to process handling affect all uses

This pattern lets you focus on the specific logic for each child process while handling the common fork/wait/signal management in one place.
```C
// ==========================================
// GENERIC CHILD PROCESS CREATOR
// COMMAND EXECUTION EXAMPLE
// ==========================================

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// ==========The Command Execution Pattern Explained
```
The Core Idea
Instead of writing fork/wait code every time you want to execute a command,<p>
you write it once in a generic function, then reuse it everywhere by passing <p>
different functions to it.
How It Works - Step by Step
<h2> 1. The Generic Child Creator (execute_in_child) </h2>

```C
int execute_in_child(t_child_executor *executor)
{
    pid = fork();
    if (pid == 0) {
        // Child: Run whatever function you gave me
        executor->child_func(executor->data);
    } else {
        // Parent: Wait for child to finish
        waitpid(pid, &status, 0);
    }
}
```
<h2> 2. Your Command-Specific Function (command_child_func)</h2>

```C
int command_child_func(void *data)
{
    t_command_data *cmd = (t_command_data *)data;
    
    // Set up redirection if needed
    if (cmd->output_fd != 1) {
        dup2(cmd->output_fd, STDOUT_FILENO);
    }
    
    // Replace process with the command
    execvp(cmd->args[0], cmd->args);
}

```

<h2> 3. Your High-Level Interface (execute_simple_command)</h2>

```C
int execute_simple_command(char **args)
{
    t_command_data cmd_data = {args, 0, 1};
    t_child_executor executor = {&cmd_data, command_child_func};
    
    return execute_in_child(&executor);  // Use the generic function!
}

```

What Happens When You Call execute_simple_command(["ls", "-l", NULL])
1. execute_simple_command() creates:
   └── t_command_data: {args: ["ls", "-l", NULL], input_fd: 0, output_fd: 1}
   └── t_child_executor: {data: &cmd_data, child_func: command_child_func}

2. execute_in_child() is called:
   └── fork() creates two processes

3. In CHILD process:
   └── Calls command_child_func(&cmd_data)
   └── command_child_func() calls execvp("ls", ["ls", "-l", NULL])
   └── Child process becomes 'ls' program

4. In PARENT process:
   └── Waits for child to finish
   └── Returns exit code
The Magic: Function Pointers
The key insight is that execute_in_child() doesn't know or care what the child process will do. It just:

Creates the child process
Calls whatever function you tell it to call
Waits for the child to finish

This means you can use the same execute_in_child() function for:
c// Simple command
execute_simple_command(["ls", NULL]);

// Command with redirection  
execute_command_with_output_redirect(["echo", "hello", NULL], file_fd);

// Command with full redirection
execute_command_with_redirection(["sort", NULL], input_fd, output_fd);
Why This is Powerful
Without this pattern - you'd repeat fork/wait everywhere:
c// To run 'ls'
pid = fork();
if (pid == 0) { execvp("ls", ls_args); }
waitpid(pid, &status, 0);

// To run 'echo' with redirection
pid = fork(); 
if (pid == 0) { 
    dup2(file_fd, 1); 
    execvp("echo", echo_args); 
}
waitpid(pid, &status, 0);

// To run 'sort' with full redirection
pid = fork();
if (pid == 0) {
    dup2(input_fd, 0);
    dup2(output_fd, 1);
    execvp("sort", sort_args);
}
waitpid(pid, &status, 0);
With this pattern - you write fork/wait once, use everywhere:

```c

// All use the same generic executor
execute_simple_command(ls_args);
execute_command_with_output_redirect(echo_args, file_fd);
execute_command_with_redirection(sort_args, input_fd, output_fd);
The Beauty of void* and Function Pointers

void *data lets you pass any type of data to the generic function
child_function_t lets you pass any function to the generic executor

```
The generic executor doesn't need to know the specifics - it just calls your function with your data
This creates a universal command executor that can handle any command execution scenario while keeping all the process management logic in one place!
