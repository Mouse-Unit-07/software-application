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
#include "command.h"
#include "get.h"
#include "set.h"
#include "solve.h"
#include "test_device.h"
#include "root.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void print_help_recursive(struct command_node const *nodes, uint32_t count,
                                 char const *prefix);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    MAX_ROOT_NODES = 10
};

static struct command_node root_commands[MAX_ROOT_NODES] =
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
        .validate = validate_time,
        .execute = execute_time
    }
};

static const uint32_t ROOT_COMMANDS_DEFAULT_COUNT = 4u;
static uint32_t root_commands_count = 4u;

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void init_root(void)
{
    enum
    {
        EXTERNAL_COMMAND_NODE_COUNT = 4
    };

    struct command_node nodes[EXTERNAL_COMMAND_NODE_COUNT];
    nodes[0] = *get_get_node();
    nodes[1] = *get_set_node();
    nodes[2] = *get_solve_node();
    nodes[3] = *get_test_node();

    uint32_t total_nodes = ROOT_COMMANDS_DEFAULT_COUNT + EXTERNAL_COMMAND_NODE_COUNT;
    total_nodes = total_nodes > MAX_ROOT_NODES ? MAX_ROOT_NODES : total_nodes;

    for (uint32_t i = ROOT_COMMANDS_DEFAULT_COUNT; i < total_nodes; i++) {
        root_commands[i] = nodes[i - ROOT_COMMANDS_DEFAULT_COUNT];
    }

    root_commands_count = total_nodes;
}

void deinit_root(void)
{
    root_commands_count = ROOT_COMMANDS_DEFAULT_COUNT;
}

struct command_node const *get_root_commands(void)
{
    return root_commands;
}

uint32_t get_root_commands_count(void)
{
    return root_commands_count;
}

struct command_match find_command_node_in_root(struct command const *cmd)
{
    return find_command_node(cmd, root_commands, root_commands_count);
}

/*----------------------------------------------------------------------------*/
/* help */
enum validation_result validate_help(struct command *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_help(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_help_recursive(root_commands, root_commands_count, "");
}

/*----------------------------------------------------------------------------*/
/* clear */
enum validation_result validate_clear(struct command *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_clear(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("\e[1;1H\e[2J");
}

/*----------------------------------------------------------------------------*/
/* faults */
enum validation_result validate_hardware_faults(struct command *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_hardware_faults(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_hardware_state();
}

/*----------------------------------------------------------------------------*/
/* time */
enum validation_result validate_time(struct command *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_time(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    uint32_t time_sec = get_current_global_time_sec();
    printf("time passed since init: %" PRIu32 " min, %" PRIu32 " sec\r\n", time_sec / 60,
           time_sec % 60);
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static void print_help_recursive(struct command_node const *nodes, uint32_t count,
                                 char const *prefix)
{
    char command_name[MAX_COMMAND_NAME_SIZE];

    for (uint32_t i = 0u; i < count; i++) {
        if ((prefix != NULL) && (prefix[0] != '\0')) {
            snprintf(command_name, sizeof(command_name), "%s %s", prefix, nodes[i].name);
        } else {
            snprintf(command_name, sizeof(command_name), "%s", nodes[i].name);
        }

        printf("%-24s %s\r\n", command_name, nodes[i].help);

        if (nodes[i].child_count > 0u) {
            print_help_recursive(nodes[i].children, nodes[i].child_count, command_name);
        }
    }
}
