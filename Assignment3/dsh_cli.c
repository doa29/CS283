#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dshlib.h"

int main()
{
    char cmd_buff[SH_CMD_MAX];
    int rc = 0;
    command_list_t clist;

    while(1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // Trim leading whitespace
        char *p = cmd_buff;
        while(*p == SPACE_CHAR || *p == '\t')
            p++;

        if(strlen(p) == 0) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }
        if(strcmp(p, EXIT_CMD) == 0)
            exit(0);

        rc = build_cmd_list(p, &clist);
        if(rc == WARN_NO_CMDS) {
            printf(CMD_WARN_NO_CMD);
        } else if(rc == ERR_TOO_MANY_COMMANDS) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
        } else if(rc == OK) {
            printf(CMD_OK_HEADER, clist.num);
            for (int i = 0; i < clist.num; i++) {
                printf("<%d> %s", i + 1, clist.commands[i].exe);
                if(strlen(clist.commands[i].args) > 0)
                    printf("[%s]", clist.commands[i].args);
                printf("\n");
            }
        }
    }
    return 0;
}
