/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : set.c                                                 */
/*                                                                            */
/* Implementation for micromouse set library                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "navigation.h"
#include "maze_solver_common.h"
#include "configuration.h"
#include "command.h"
#include "set.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    SET_COMMAND_TOKEN_COUNT = ROOT_COMMAND_TOKEN_COUNT + 1
};

enum
{
    MAZE_SOLVER_PARAMETER_COUNT = 5,
    MOUSE_PHYSICAL_PARAMETER_COUNT = 6,
    MAZE_PHYSICAL_PARAMETER_COUNT = 2
};

static const struct command_node set_commands[] =
{
    {
        .name = "solver-default",
        .help = "Use default solver configuration",
        .validate = validate_set_solver_default,
        .execute = execute_set_solver_default
    },
    {
        .name = "solver-test",
        .help = "Use test solver configuration; optionally pass: "
            "maze_size, total_timeout_sec, move_forward_time_sec, "
            "rotate_90_deg_time_sec, rotate_180_deg_time_sec",
        .validate = validate_set_solver_test,
        .execute = execute_set_solver_test
    },
    {
        .name = "mouse-physical-default",
        .help = "Use default mouse physical parameters",
        .validate = validate_set_mouse_physical_default,
        .execute = execute_set_mouse_physical_default
    },
    {
        .name = "mouse-physical-test",
        .help = "Use test mouse physical parameters; optionally pass: "
            "wheel_diameter_mm, wheel_base_mm, max_motor_rpm, "
            "encoder_events_per_revolution, motor_pinion_gear_teeth, "
            "wheel_gear_teeth",
        .validate = validate_set_mouse_physical_test,
        .execute = execute_set_mouse_physical_test
    },
    {
        .name = "maze-physical-default",
        .help = "Use default maze physical parameters",
        .validate = validate_set_maze_physical_default,
        .execute = execute_set_maze_physical_default
    },
    {
        .name = "maze-physical-test",
        .help = "Use test maze physical parameters; optionally pass: "
            "post_size_mm, wall_size_mm",
        .validate = validate_set_maze_physical_test,
        .execute = execute_set_maze_physical_test
    },
};

static const struct command_node set_node =
{
    .name = "set",
    .help = "Select configuration values to use w/ or w/o new values for test configs",
    .validate = validate_set,
    .children = set_commands,
    .child_count = sizeof(set_commands) / sizeof(set_commands[0])
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
const struct command_node *get_set_node(void)
{
    return &set_node;
}

const struct command_node *get_set_commands(void)
{
    return set_commands;
}

uint32_t get_set_commands_count(void)
{
    return sizeof(set_commands) / sizeof(set_commands[0]);
}

/*----------------------------------------------------------------------------*/
/* set */
enum validation_result validate_set(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
enum validation_result validate_set_solver_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_solver_default(struct command const *cmd)
{
    (void)cmd;

    set_maze_solver_config(get_saved_default_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* set solver-test */
enum validation_result validate_set_solver_test(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + MAZE_SOLVER_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == max_token_count) {
        struct maze_solver_config cfg;
        cfg.maze_size = (uint32_t)strtoul(cmd->tokens[param_0_offset], NULL, 10);

        if (cfg.maze_size > 16u) {
            cmd->bad_parameter_index = param_0_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_solver_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_maze_solver_config(get_saved_test_maze_solver_config());
        return;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;
    uint32_t param_2_offset = base_offset + PARAM_2_OFFSET;
    uint32_t param_3_offset = base_offset + PARAM_3_OFFSET;
    uint32_t param_4_offset = base_offset + PARAM_4_OFFSET;

    struct maze_solver_config cfg;
    cfg.maze_size = (uint32_t)strtoul(cmd->tokens[param_0_offset], NULL, 10);
    cfg.total_timeout_sec = (uint32_t)strtoul(cmd->tokens[param_1_offset], NULL, 10);
    cfg.move_forward_time_sec = (uint32_t)strtoul(cmd->tokens[param_2_offset], NULL, 10);
    cfg.rotate_90_deg_time_sec = (uint32_t)strtoul(cmd->tokens[param_3_offset], NULL, 10);
    cfg.rotate_180_deg_time_sec = (uint32_t)strtoul(cmd->tokens[param_4_offset], NULL, 10);

    save_maze_solver_config_as_test(cfg);
    set_maze_solver_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set mouse-physical-default */
enum validation_result validate_set_mouse_physical_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_mouse_physical_default(struct command const *cmd)
{
    (void)cmd;

    calculate_mouse_params(get_saved_default_mouse_physical_params());
    calculate_navigation_params();
}

/*----------------------------------------------------------------------------*/
/* set mouse-physical-test */
enum validation_result validate_set_mouse_physical_test(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + MOUSE_PHYSICAL_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_mouse_physical_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        calculate_mouse_params(get_saved_test_mouse_physical_params());
        calculate_navigation_params();
        return;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;

    struct mouse_physical_params p = {0};
    p.wheel_diameter_mm = strtof(cmd->tokens[base_offset + PARAM_0_OFFSET], NULL);
    p.wheel_base_mm = strtof(cmd->tokens[base_offset + PARAM_1_OFFSET], NULL);
    p.max_motor_rpm = strtof(cmd->tokens[base_offset + PARAM_2_OFFSET], NULL);
    p.encoder_events_per_revolution = strtof(cmd->tokens[base_offset + PARAM_3_OFFSET], NULL);
    p.motor_pinion_gear_teeth = strtof(cmd->tokens[base_offset + PARAM_4_OFFSET], NULL);
    p.wheel_gear_teeth = strtof(cmd->tokens[base_offset + PARAM_5_OFFSET], NULL);

    save_mouse_physical_params_as_test(p);
    calculate_mouse_params(p);
    calculate_navigation_params();
}

/*----------------------------------------------------------------------------*/
/* set maze-physical-default */
enum validation_result validate_set_maze_physical_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_maze_physical_default(struct command const *cmd)
{
    (void)cmd;

    calculate_maze_params(get_saved_default_maze_physical_params());
    calculate_navigation_params();
}

/*----------------------------------------------------------------------------*/
/* set maze-physical-test */
enum validation_result validate_set_maze_physical_test(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + MAZE_PHYSICAL_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_maze_physical_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        calculate_maze_params(get_saved_test_maze_physical_params());
        calculate_navigation_params();
        return;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;

    struct maze_physical_params p = {0};
    p.post_size_mm = strtof(cmd->tokens[base_offset + PARAM_0_OFFSET], NULL);
    p.wall_size_mm = strtof(cmd->tokens[base_offset + PARAM_1_OFFSET], NULL);

    save_maze_physical_params_as_test(p);
    calculate_maze_params(p);
    calculate_navigation_params();
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
