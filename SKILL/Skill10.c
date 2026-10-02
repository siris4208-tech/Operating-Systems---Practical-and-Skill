  GNU nano 8.7.1                                                                                                 Skill10.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>

int validName(char *name)
{
    if (!(isalpha(name[0]) || name[0] == '_'))
        return 0;

    for (int i = 1; name[i]; i++)
    {
        if (!(isalnum(name[i]) || name[i] == '_'))
            return 0;
    }
    return 1;
}

int main()
{
    char input[200];
    char cwd[PATH_MAX];
    char lastPath[PATH_MAX];

    while (1)
    {
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            strcpy(lastPath, cwd);
            printf("\nCurrent Directory: %s\n", cwd);
        }
        else
        {
            perror("getcwd");
            break;
        }

        printf("\nCommands:\n");
        printf("  pwd\n");
        printf("  export NAME=value\n");
        printf("  child\n");
        printf("  exit\n");

        // --- Inferred Input Reading Logic (Cropped between images) ---
        printf("Enter command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        input[strcspn(input, "\n")] = '\0'; // Remove newline

        if (strcmp(input, "pwd") == 0)
        {
            printf("%s\n", cwd);
        }
        else if (strcmp(input, "exit") == 0)
        {
            printf("Exiting program...\n");
            break;
        }
        // -------------------------------------------------------------

        /* Child Process */
        else if (strcmp(input, "child") == 0)
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                execl("/usr/bin/env", "env", NULL);
                perror("exec");
                exit(1);
            }

            wait(NULL);
        }

        /* Export Command */
        else if (strncmp(input, "export ", 7) == 0)
        {
            char *expr = input + 7;
            char *eq = strchr(expr, '=');

            if (eq == NULL)
            {
                printf("Invalid syntax. Use: export NAME=value\n");
                continue;
            }

            *eq = '\0';
            char *name = expr;
            char *value = eq + 1;

            if (!validName(name))
            {
                printf("Invalid variable name.\n");
                continue;
            }

            setenv(name, value, 1);
            printf("Exported: %s=%s\n", name, getenv(name));
        }
    }

    return 0;
}
