/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : commands.c                                            */
/*                                                                            */
/* Implementation for micromouse commands library                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "command.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
enum validation_result validate_parameterless_command(struct command const *cmd,
                                                      uint32_t token_count)
{
    if (cmd->token_count > token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

struct command_match find_command_node(struct command const *cmd,
                                       struct command_node const *commands, uint32_t command_count)
{
    struct command_match match = {0};
    struct command_node const *current = commands;
    uint32_t count = command_count;

    for (uint32_t token = 0u; token < cmd->token_count; token++) {
        struct command_node const *found = NULL;

        for (uint32_t i = 0u; i < count; i++) {
            if (strcmp(current[i].name, cmd->tokens[token]) == 0) {
                found = &current[i];
                current = found->children;
                count = found->child_count;
                break;
            }
        }

        if (found == NULL) {
            return match;
        }

        match.node = found;
        match.depth = token + 1u;
    }

    return match;
}

void print_command_help(struct command_node const *commands, uint32_t count)
{
    for (uint32_t i = 0u; i < count; i++) {
        printf("%-32s %s\r\n", commands[i].name, commands[i].help);
    }
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */