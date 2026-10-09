#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "history.h"


/* =========================================================
   BUILTIN: cd
   ========================================================= */

static int builtin_cd(command_t *cmd)
{
    const char *directory;

    if (cmd->argc == 1)
    {
        directory = getenv("HOME");

        if (directory == NULL)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return -1;
        }
    }
    else if (cmd->argc == 2)
    {
        directory = cmd->argv[1];
    }
    else
    {
        fprintf(stderr, "cd: too many arguments\n");
        return -1;
    }

    if (chdir(directory) != 0)
    {
        perror("cd");
        return -1;
    }

    return 0;
}


/* =========================================================
   BUILTIN: pwd
   ========================================================= */

static int builtin_pwd(command_t *cmd)
{
    char current_directory[4096];

    if (cmd->argc > 1)
    {
        fprintf(stderr, "pwd: too many arguments\n");
        return -1;
    }

    if (getcwd(current_directory, sizeof(current_directory)) == NULL)
    {
        perror("pwd");
        return -1;
    }

    printf("%s\n", current_directory);

    return 0;
}


/* =========================================================
   BUILTIN: echo
   ========================================================= */

static int builtin_echo(command_t *cmd)
{
    for (int i = 1; i < cmd->argc; i++)
    {
        printf("%s", cmd->argv[i]);

        if (i < cmd->argc - 1)
            printf(" ");
    }

    printf("\n");

    return 0;
}


/* =========================================================
   BUILTIN: exit
   ========================================================= */

static int builtin_exit(command_t *cmd)
{
    if (cmd->argc > 1)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return -1;
    }

    return 1;
}


/* =========================================================
   BUILTIN: history
   ========================================================= */

static int builtin_history(command_t *cmd)
{
    if (cmd->argc > 1)
    {
        fprintf(stderr, "history: too many arguments\n");
        return -1;
    }

    print_history();

    return 0;
}


/* =========================================================
   BUILTIN: help
   ========================================================= */

static int builtin_help(command_t *cmd)
{
    if (cmd->argc > 1)
    {
        fprintf(stderr, "help: too many arguments\n");
        return -1;
    }

    printf("Shellforge built-in commands:\n");
    printf("  cd       Change the current directory\n");
    printf("  pwd      Print the current working directory\n");
    printf("  echo     Display text\n");
    printf("  history  Display command history\n");
    printf("  help     Display this help message\n");
    printf("  export   Set an environment variable\n");
    printf("  unset    Remove an environment variable\n");
    printf("  exit     Exit Shellforge\n");

    return 0;
}


/* =========================================================
   BUILTIN: export
   ========================================================= */

static int builtin_export(command_t *cmd)
{
    if (cmd->argc != 2)
    {
        fprintf(stderr, "export: usage: export NAME=value\n");
        return -1;
    }

    char *equals = strchr(cmd->argv[1], '=');

    if (equals == NULL)
    {
        fprintf(stderr, "export: usage: export NAME=value\n");
        return -1;
    }

    *equals = '\0';

    char *name = cmd->argv[1];
    char *value = equals + 1;

    if (strlen(name) == 0)
    {
        fprintf(stderr, "export: invalid variable name\n");
        *equals = '=';
        return -1;
    }

    if (setenv(name, value, 1) != 0)
    {
        perror("export");
        *equals = '=';
        return -1;
    }

    *equals = '=';

    return 0;
}


/* =========================================================
   BUILTIN: unset
   ========================================================= */

static int builtin_unset(command_t *cmd)
{
    if (cmd->argc != 2)
    {
        fprintf(stderr, "unset: usage: unset NAME\n");
        return -1;
    }

    if (unsetenv(cmd->argv[1]) != 0)
    {
        perror("unset");
        return -1;
    }

    return 0;
}


/* =========================================================
   CHECK WHETHER COMMAND IS A BUILTIN
   ========================================================= */

int is_builtin(const command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return 0;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "echo") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "exit") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "history") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "help") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "export") == 0)
        return 1;

    if (strcmp(cmd->argv[0], "unset") == 0)
        return 1;

    return 0;
}


/* =========================================================
   EXECUTE BUILTIN
   ========================================================= */

int execute_builtin(command_t *cmd)
{
    if (cmd == NULL || cmd->argc == 0)
        return -1;

    if (strcmp(cmd->argv[0], "cd") == 0)
        return builtin_cd(cmd);

    if (strcmp(cmd->argv[0], "pwd") == 0)
        return builtin_pwd(cmd);

    if (strcmp(cmd->argv[0], "echo") == 0)
        return builtin_echo(cmd);

    if (strcmp(cmd->argv[0], "exit") == 0)
        return builtin_exit(cmd);

    if (strcmp(cmd->argv[0], "history") == 0)
        return builtin_history(cmd);

    if (strcmp(cmd->argv[0], "help") == 0)
        return builtin_help(cmd);

    if (strcmp(cmd->argv[0], "export") == 0)
        return builtin_export(cmd);

    if (strcmp(cmd->argv[0], "unset") == 0)
        return builtin_unset(cmd);

    return -1;
}
