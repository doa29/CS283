#include "dshlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include "dragon.h"

// build_cmd_buff parses input into a cmd_buff_t structure
int build_cmd_buff(char *input, cmd_buff_t *buff) {
    buff->_cmd_buffer = strdup(input);
    if (!buff->_cmd_buffer) {
        perror("strdup");
        return -1;
    }
    
    buff->argc = 0;
    char *p = buff->_cmd_buffer;
    
    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0') break;

        char *token = NULL;
        if (*p == '\"') {
            p++; 
            token = p;
            while (*p && *p != '\"') p++;
            if (*p == '\"') {
                *p = '\0';
                p++;
            }
        } else {
            token = p;
            while (*p && !isspace((unsigned char)*p)) p++;
            if (*p) {
                *p = '\0';
                p++;
            }
        }

        if (token && buff->argc < CMD_ARGV_MAX - 1) {
            buff->argv[buff->argc++] = token;
        }
    }
    buff->argv[buff->argc] = NULL;
    
    if (buff->argc == 0) {
        free(buff->_cmd_buffer);
        buff->_cmd_buffer = NULL;
        return -1;
    }
    
    return 0;
}

// Main command execution loop
void exec_local_cmd_loop() {
    char input[SH_CMD_MAX];
    int last_rc = 0;

    while (1) {
        printf(SH_PROMPT);
        fflush(stdout);

        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strlen(input) == 0)
            continue;

        cmd_buff_t buff;
        if (build_cmd_buff(input, &buff) < 0)
            continue;

        if (buff.argc == 0) {
            free(buff._cmd_buffer);
            continue;
        }

        if (strcmp(buff.argv[0], "exit") == 0) {
            free(buff._cmd_buffer);
            break;
        }

        if (strcmp(buff.argv[0], "cd") == 0) {
            if (buff.argc == 2) {
                if (chdir(buff.argv[1]) != 0)
                    perror("chdir");
            }
            free(buff._cmd_buffer);
            continue;
        }

        if (strcmp(buff.argv[0], "rc") == 0) {
            printf("%d\n", last_rc);
            free(buff._cmd_buffer);
            continue;
        }

        if (strcmp(buff.argv[0], "dragon") == 0) {
            print_dragon();
            free(buff._cmd_buffer);
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            free(buff._cmd_buffer);
            continue;
        }
        if (pid == 0) {
            execvp(buff.argv[0], buff.argv);
            perror("execvp");
            exit(errno);
        } else {
            int status;
            waitpid(pid, &status, 0);
            last_rc = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        }

        free(buff._cmd_buffer);
    }
}
