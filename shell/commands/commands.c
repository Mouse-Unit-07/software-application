/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : commands.c                                            */
/*                                                                            */
/* Implementation for micromouse commands library                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fault_detector.h"
#include "commands.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
static const struct command_table_entry command_table[] =
{
    {
        .name = "help",
        .help = "Display available commands",
        .validate = validate_help,
        .execute = execute_help
    },
    {
        .name = "clear",
        .help = "Clear the console",
        .validate = validate_clear,
        .execute = execute_clear
    },
    {
        .name = "faults",
        .help = "Display all hardware faults",
        .validate = validate_hardware_faults,
        .execute = execute_hardware_faults
    },
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
uint32_t get_command_table_size(void)
{
    return sizeof(command_table) / sizeof(command_table[0]);
}

struct command_table_entry get_command_table_entry_at_index(uint32_t index)
{
    struct command_table_entry entry = {{0}};

    if (index >= get_command_table_size()) {
        return entry;
    }

    return command_table[index];
}

enum validation_result validate_help(struct command const *cmd)
{
    if (strcmp(cmd->command, "help") != 0) {
        return COMMAND_VALIDATION_NOT_MATCHED;
    }

    if (cmd->parameter_count != 0) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_help(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("\r\n");
    printf("Available Commands\r\n");
    printf("------------------\r\n");

    for (uint32_t i = 0u; i < get_command_table_size(); i++) {
        printf("%-16s %s\r\n", command_table[i].name, command_table[i].help);
    }

    printf("\r\n");
}

enum validation_result validate_clear(struct command const *cmd)
{
    if (strcmp(cmd->command, "clear") != 0) {
        return COMMAND_VALIDATION_NOT_MATCHED;
    }

    if (cmd->parameter_count != 0) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_clear(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("\e[1;1H\e[2J"); 
}

enum validation_result validate_hardware_faults(struct command const *cmd)
{
    if (strcmp(cmd->command, "faults") != 0) {
        return COMMAND_VALIDATION_NOT_MATCHED;
    }

    if (cmd->parameter_count != 0) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_hardware_faults(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_hardware_state();
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
