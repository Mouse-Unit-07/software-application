/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : commands.c                                            */
/*                                                                            */
/* Implementation for micromouse commands library                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "fault_detector.h"
#include "global_time.h"
#include "commands.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_parameterless_command(struct command const *cmd);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
static const struct command_node root_commands[] =
{
    {
        .name = "help",
        .help = "Display available commands",
        .validate = validate_help,
        .execute = execute_help
    },
    {
        .name = "clear",
        .help = "Clear console",
        .validate = validate_clear,
        .execute = execute_clear
    },
    {
        .name = "faults",
        .help = "Display all hardware faults",
        .validate = validate_hardware_faults,
        .execute = execute_hardware_faults
    },
    {
        .name = "time",
        .help = "Display current time",
        .validate = validate_get_time,
        .execute = execute_get_time
    }
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
struct command_node const *get_command_tree_root(void)
{
    return root_commands;
}

struct command_node const *find_command_node(struct command const *cmd)
{
    struct command_node const *root = get_command_tree_root();

    for (uint32_t i = 0u; i < get_command_tree_root_count(); i++) {
        if (strcmp(root[i].name, cmd->command) == 0) {
            return &root[i];
        }
    }

    return NULL;
}

uint32_t get_command_tree_root_count(void)
{
    return sizeof(root_commands) / sizeof(root_commands[0]);
}

/*----------------------------------------------------------------------------*/
/* help */
enum validation_result validate_help(struct command const *cmd)
{
    return validate_parameterless_command(cmd);
}

void execute_help(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    struct command_node const *root = get_command_tree_root();

    for (uint32_t i = 0u; i < get_command_tree_root_count(); i++) {
        printf("%-16s %s\r\n", root[i].name, root[i].help);
    }
}

/*----------------------------------------------------------------------------*/
/* clear */
enum validation_result validate_clear(struct command const *cmd)
{
    return validate_parameterless_command(cmd);
}

void execute_clear(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("\e[1;1H\e[2J"); 
}

/*----------------------------------------------------------------------------*/
/* faults */
enum validation_result validate_hardware_faults(struct command const *cmd)
{
    return validate_parameterless_command(cmd);
}

void execute_hardware_faults(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_hardware_state();
}

/*----------------------------------------------------------------------------*/
/* time */
enum validation_result validate_get_time(struct command const *cmd)
{
    return validate_parameterless_command(cmd);
}

void execute_get_time(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    uint32_t time_sec = get_current_global_time_sec();
    printf("time passed since init: %" PRIu32 " min, %" PRIu32 " sec\r\n", 
           time_sec / 60, time_sec % 60);
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_parameterless_command(struct command const *cmd)
{
    if (cmd->parameter_count != 0) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}
