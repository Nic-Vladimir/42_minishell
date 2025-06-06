<h2> 1)Define your data structure: </h2>

```bash
ctypedef struct s_command_data
{
    char **args;
    t_env *env;
    int input_fd;
    int output_fd;
} t_command_data;
```

<h2> 2) Define your function: </h2>

```bash
cstatic int command_child_func(void *data)
{
    t_command_data *cmd = (t_command_data *)data;
    // Execute command logic
    return execve(cmd->args[0], cmd->args, env_to_array(cmd->env));
}
```

<h2> 3)Use the generic executor: </h2>

```bash
cint execute_command(char **args, t_env *env)
{
    t_command_data cmd_data = {args, env, 0, 1};
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
