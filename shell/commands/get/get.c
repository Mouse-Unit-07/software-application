/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : get.c                                                 */
/*                                                                            */
/* Implementation for micromouse get library                                  */
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
#include "maze_solver_common.h"
#include "configuration.h"
#include "command.h"
#include "get.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void print_maze_solver_config(struct maze_solver_config cfg);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    GET_COMMAND_TOKEN_COUNT = ROOT_COMMAND_TOKEN_COUNT + 1
};

static const struct command_node get_commands[] =
{
    {
        .name = "solver-default",
        .help = "Display default solver configuration",
        .validate = validate_get_solver_default,
        .execute = execute_get_solver_default
    },
    {
        .name = "solver-test",
        .help = "Display test solver configuration",
        .validate = validate_get_solver_test,
        .execute = execute_get_solver_test
    },
    {
        .name = "solver-current",
        .help = "Display current solver configuration",
        .validate = validate_get_solver_current,
        .execute = execute_get_solver_current
    }
};

static const struct command_node get_node =
{
    .name = "get",
    .help = "Read configuration values",
    .validate = validate_get,
    .children = get_commands,
    .child_count = sizeof(get_commands) / sizeof(get_commands[0])
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
const struct command_node *get_get_node(void)
{
    return &get_node;
}

const struct command_node *get_get_commands(void)
{
    return get_commands;
}

uint32_t get_get_commands_count(void)
{
    return sizeof(get_commands) / sizeof(get_commands[0]);
}

/*----------------------------------------------------------------------------*/
/* get */
enum validation_result validate_get(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
enum validation_result validate_get_solver_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_default(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_default_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* get solver-test */
enum validation_result validate_get_solver_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_test(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_test_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* get solver-current */
enum validation_result validate_get_solver_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_current(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static void print_maze_solver_config(struct maze_solver_config cfg)
{
    printf("maze_size                = %" PRIu32 "\r\n", cfg.maze_size);
    printf("total_timeout_sec        = %" PRIu32 "\r\n", cfg.total_timeout_sec);
    printf("move_forward_time_sec    = %" PRIu32 "\r\n", cfg.move_forward_time_sec);
    printf("rotate_90_deg_time_sec   = %" PRIu32 "\r\n", cfg.rotate_90_deg_time_sec);
    printf("rotate_180_deg_time_sec  = %" PRIu32 "\r\n", cfg.rotate_180_deg_time_sec);
}
