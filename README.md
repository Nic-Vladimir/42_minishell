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

```c
// ==========================================
// GENERIC CHILD PROCESS CREATOR
// COMMAND EXECUTION EXAMPLE
// ==========================================

#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

// ==========================================
// 1. THE GENERIC CHILD PROCESS CREATOR
// ==========================================

// Function pointer type - ALL child functions must match this signature
typedef int (*child_function_t)(void *data);

// Generic structure to hold data and function
typedef struct s_child_executor {
    void *data;                    // Pointer to your specific data
    child_function_t child_func;   // Pointer to your specific function
} t_child_executor;

// THE MAIN FUNCTION - Creates child process and runs your function
int execute_in_child(t_child_executor *executor)
{
    pid_t pid;
    int status;
    
    printf("[PARENT] Creating child process...\n");
    
    // Create a child process
    pid = fork();
    
    if (pid == -1) {
        perror("fork");
        return -1;
    }
    else if (pid == 0) {
        // ==========================================
        // CHILD PROCESS CODE
        // ==========================================
        
        printf("[CHILD] Child process running (PID: %d)\n", getpid());
        
        // Call YOUR function with YOUR data
        int result = executor->child_func(executor->data);
        
        // Child process MUST exit (never return to main)
        exit(result);
    }
    else {
        // ==========================================
        // PARENT PROCESS CODE
        // ==========================================
        
        printf("[PARENT] Child PID: %d, waiting for completion...\n", pid);
        
        // Wait for child to finish
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf("[PARENT] Child exited with code: %d\n", exit_code);
            return exit_code;
        }
        
        return -1;
    }
}

// ==========================================
// 2. COMMAND EXECUTION SPECIFIC CODE
// ==========================================

// Data structure for command execution
typedef struct s_command_data {
    char **args;        // Command arguments: ["ls", "-la", NULL]
    int input_fd;       // Where to read from (0 = stdin)
    int output_fd;      // Where to write to (1 = stdout)
} t_command_data;

// Function that runs in the child process
int command_child_func(void *data)
{
    // Cast void* back to our specific type
    t_command_data *cmd = (t_command_data *)data;
    
    printf("[CHILD] About to execute command: ");
    for (int i = 0; cmd->args[i] != NULL; i++) {
        printf("%s ", cmd->args[i]);
    }
    printf("\n");
    
    // Set up input/output redirection if needed
    if (cmd->input_fd != 0) {
        printf("[CHILD] Redirecting input from fd %d\n", cmd->input_fd);
        dup2(cmd->input_fd, STDIN_FILENO);
        close(cmd->input_fd);
    }
    
    if (cmd->output_fd != 1) {
        printf("[CHILD] Redirecting output to fd %d\n", cmd->output_fd);
        dup2(cmd->output_fd, STDOUT_FILENO);
        close(cmd->output_fd);
    }
    
    // Replace this process with the new program
    execvp(cmd->args[0], cmd->args);
    
    // If we reach here, execvp failed
    perror("[CHILD] execvp failed");
    return 127;  // Standard "command not found" exit code
}

// ==========================================
// 3. HIGH-LEVEL COMMAND EXECUTION FUNCTIONS
// ==========================================

// Simple command execution (no redirection)
int execute_simple_command(char **args)
{
    printf("=== EXECUTING SIMPLE COMMAND ===\n");
    
    // Step 1: Prepare command data
    t_command_data cmd_data = {
        .args = args,
        .input_fd = 0,    // Use standard input
        .output_fd = 1    // Use standard output
    };
    
    // Step 2: Prepare executor
    t_child_executor executor = {
        .data = &cmd_data,              // Our command data
        .child_func = command_child_func // Our command function
    };
    
    // Step 3: Execute using generic function
    return execute_in_child(&executor);
}

// Command execution with output redirection
int execute_command_with_output_redirect(char **args, int output_fd)
{
    printf("=== EXECUTING COMMAND WITH OUTPUT REDIRECT ===\n");
    
    t_command_data cmd_data = {
        .args = args,
        .input_fd = 0,
        .output_fd = output_fd  // Redirect output to this file descriptor
    };
    
    t_child_executor executor = {
        .data = &cmd_data,
        .child_func = command_child_func
    };
    
    return execute_in_child(&executor);
}

// Command execution with full redirection
int execute_command_with_redirection(char **args, int input_fd, int output_fd)
{
    printf("=== EXECUTING COMMAND WITH FULL REDIRECTION ===\n");
    
    t_command_data cmd_data = {
        .args = args,
        .input_fd = input_fd,
        .output_fd = output_fd
    };
    
    t_child_executor executor = {
        .data = &cmd_data,
        .child_func = command_child_func
    };
    
    return execute_in_child(&executor);
}

// ==========================================
// 4. DEMONSTRATION
// ==========================================

int main(void)
{
    printf("GENERIC CHILD PROCESS CREATOR - COMMAND EXECUTION DEMO\n");
    printf("======================================================\n\n");
    
    // Example 1: Simple command
    printf("1. Simple command execution:\n");
    char *ls_args[] = {"ls", "-l", NULL};
    int result1 = execute_simple_command(ls_args);
    printf("Command exited with code: %d\n\n", result1);
    
    // Example 2: Command with output redirection
    printf("2. Command with output redirection:\n");
    int output_fd = open("output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (output_fd != -1) {
        char *echo_args[] = {"echo", "Hello World", NULL};
        int result2 = execute_command_with_output_redirect(echo_args, output_fd);
        close(output_fd);
        printf("Command exited with code: %d\n", result2);
        printf("Output written to output.txt\n\n");
    }
    
    // Example 3: Multiple commands using the same generic function
    printf("3. Multiple different commands:\n");
    
    char *date_args[] = {"date", NULL};
    printf("Running 'date':\n");
    execute_simple_command(date_args);
    
    char *whoami_args[] = {"whoami", NULL};
    printf("Running 'whoami':\n");
    execute_simple_command(whoami_args);
    
    char *pwd_args[] = {"pwd", NULL};
    printf("Running 'pwd':\n");
    execute_simple_command(pwd_args);
    
    return 0;
}

// ==========================================
// HOW IT WORKS - DETAILED BREAKDOWN
// ==========================================

/*
STEP-BY-STEP EXECUTION FLOW:

1. You call: execute_simple_command(["ls", "-l", NULL])

2. execute_simple_command() creates:
   - t_command_data with your args
   - t_child_executor pointing to data and command_child_func

3. execute_in_child() is called:
   - fork() creates child process
   - Child runs command_child_func() with your data
   - Parent waits for child to complete

4. In child process, command_child_func():
   - Sets up any needed redirection
   - Calls execvp("ls", ["ls", "-l", NULL])
   - Process image is replaced with 'ls' program

5. Parent receives exit code and returns it

THE POWER:
- execute_in_child() handles ALL the fork/wait logic
- command_child_func() handles command-specific setup
- You can create different child functions for different needs
- Same generic executor works for ALL command types

REUSABILITY:
- Want to run "cat file.txt"? Use execute_simple_command()
- Want to run "ls > output.txt"? Use execute_command_with_output_redirect()
- Want to run "sort < input.txt > output.txt"? Use execute_command_with_redirection()
- All use the SAME generic execute_in_child() function!

EXTENSIBILITY:
- Need a different type of child process? Write a new child function
- Need different data? Create a new data structure
- The generic executor remains unchanged
*/

```
