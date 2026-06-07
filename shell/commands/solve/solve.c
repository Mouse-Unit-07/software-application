/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : solve.c                                               */
/*                                                                            */
/* Implementation for micromouse solve library                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "maze_solver_common.h"
#include "wall_follower.h"
#include "partial_flood_fill.h"
#include "command.h"
#include "solve.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    SOLVE_COMMAND_TOKEN_COUNT = ROOT_COMMAND_TOKEN_COUNT + 1,
};

enum
{
    WALLFOLLOWER_PARAM_COUNT_MIN = 1,
    WALLFOLLOWER_PARAM_COUNT_MAX = 2,
    FLOODFILL_PARAM_COUNT_MIN = 0,
    FLOODFILL_PARAM_COUNT_MAX = 1,
};

static const struct command_node solve_commands[] =
{
    {
        .name = "wallfollower",
        .help = "Run wall follower: left|right [enable]",
        .validate = validate_solve_wallfollower,
        .execute = execute_solve_wallfollower
    },
    {
        .name = "floodfill",
        .help = "Run partial flood fill [enable]",
        .validate = validate_solve_floodfill,
        .execute = execute_solve_floodfill
    }
};

static const struct command_node solve_node =
{
    .name = "solve",
    .help = "Run maze solving algorithms",
    .validate = validate_solve,
    .children = solve_commands,
    .child_count = sizeof(solve_commands) / sizeof(solve_commands[0])
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
const struct command_node *get_solve_node(void)
{
    return &solve_node;
}

const struct command_node *get_solve_commands(void)
{
    return solve_commands;
}

uint32_t get_solve_commands_count(void)
{
    return sizeof(solve_commands) / sizeof(solve_commands[0]);
}

/*----------------------------------------------------------------------------*/
/* solve */
enum validation_result validate_solve(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* solve wallfollower */
enum validation_result validate_solve_wallfollower(struct command *cmd)
{
    uint32_t min_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MIN;
    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MAX;

    if (cmd->token_count < min_token_count) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > max_token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;

    if ((strcmp(cmd->tokens[param_0_offset], "left") != 0)
        && (strcmp(cmd->tokens[param_0_offset], "right") != 0)) {
        cmd->bad_parameter_index = param_0_offset;
        return COMMAND_VALIDATION_BAD_PARAMETER;
    }

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_1_offset], "enable") != 0) {
            cmd->bad_parameter_index = param_1_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_solve_wallfollower(struct command const *cmd)
{
    bool enable_print = false;
    enum wall_follower_mode mode;

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;

    if (strcmp(cmd->tokens[param_0_offset], "left") == 0) {
        mode = WALL_FOLLOWER_LEFT;
    } else {
        mode = WALL_FOLLOWER_RIGHT;
    }

    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MAX;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_1_offset], "enable") == 0) {
            enable_print = true;
        }
    }

    run_wall_follower(mode, enable_print);
}

/*----------------------------------------------------------------------------*/
/* solve floodfill */
enum validation_result validate_solve_floodfill(struct command *cmd)
{
    uint32_t min_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MIN;
    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MAX;

    if (cmd->token_count < min_token_count) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > max_token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_0_offset], "enable") != 0) {
            cmd->bad_parameter_index = param_0_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_solve_floodfill(struct command const *cmd)
{
    bool enable_print = false;

    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MAX;
    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_0_offset], "enable") == 0) {
            enable_print = true;
        }
    }

    run_partial_flood_fill(enable_print);
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
