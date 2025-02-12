#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    clist->num = 0;
    char *token = strtok(cmd_line, PIPE_STRING);
    while(token != NULL) {
        if(clist->num >= CMD_MAX)
            return ERR_TOO_MANY_COMMANDS;

        // Trim leading whitespace
        while(*token == ' ' || *token == '\t')
            token++;

        // Trim trailing whitespace
        int len = strlen(token);
        while(len > 0 && (token[len - 1] == ' ' || token[len - 1] == '\t')) {
            token[len - 1] = '\0';
            len--;
        }
        if(strlen(token) == 0) {
            token = strtok(NULL, PIPE_STRING);
            continue;
        }

        command_t *cmd = &clist->commands[clist->num];
        memset(cmd->exe, 0, EXE_MAX);
        memset(cmd->args, 0, ARG_MAX);

        // Split token into words based on space or tab.
        char *word = strtok(token, " \t");
        if(word == NULL) {
            token = strtok(NULL, PIPE_STRING);
            continue;
        }
        if(strlen(word) >= EXE_MAX)
            return ERR_CMD_OR_ARGS_TOO_BIG;
        strcpy(cmd->exe, word);

        // The rest of the words are considered arguments.
        char *arg_word = strtok(NULL, " \t");
        int firstArg = 1;
        while(arg_word != NULL) {
            if(strlen(cmd->args) + strlen(arg_word) + (firstArg ? 0 : 1) >= ARG_MAX)
                return ERR_CMD_OR_ARGS_TOO_BIG;
            if(!firstArg)
                strcat(cmd->args, " ");
            strcat(cmd->args, arg_word);
            firstArg = 0;
            arg_word = strtok(NULL, " \t");
        }
        clist->num++;
        token = strtok(NULL, PIPE_STRING);
    }
    if(clist->num == 0)
        return WARN_NO_CMDS;
    return OK;
}
