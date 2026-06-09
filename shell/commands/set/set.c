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
static enum validation_result validate_just_command_param_count(struct command *cmd,
                                                                uint32_t param_count);
static enum validation_result validate_move_forward_command(struct command *cmd);
static struct move_forward_control_config parse_move_forward_command(struct command const *cmd);
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
    MAZE_PHYSICAL_PARAMETER_COUNT = 2,
    MOVE_FORWARD_CONTROL_PARAMETER_COUNT = 11,
    ROTATE_CONTROL_PARAMETER_COUNT = 8,
    FRONT_WALL_PARAMETER_COUNT = 2,
    SIDE_WALL_PARAMETER_COUNT = 4
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
    {
        .name = "move-forward-no-wall-default",
        .help = "Use default no-wall move-forward config",
        .validate = validate_set_move_forward_no_wall_default,
        .execute = execute_set_move_forward_no_wall_default
    },
    {
        .name = "move-forward-no-wall-test",
        .help = "Use test no-wall move-forward config",
        .validate = validate_set_move_forward_no_wall_test,
        .execute = execute_set_move_forward_no_wall_test
    },
    {
        .name = "move-forward-one-wall-default",
        .help = "Use default one-wall move-forward config",
        .validate = validate_set_move_forward_one_wall_default,
        .execute = execute_set_move_forward_one_wall_default
    },
    {
        .name = "move-forward-one-wall-test",
        .help = "Use test one-wall move-forward config",
        .validate = validate_set_move_forward_one_wall_test,
        .execute = execute_set_move_forward_one_wall_test
    },
    {
        .name = "move-forward-both-wall-default",
        .help = "Use default both-wall move-forward config",
        .validate = validate_set_move_forward_both_wall_default,
        .execute = execute_set_move_forward_both_wall_default
    },
    {
        .name = "move-forward-both-wall-test",
        .help = "Use test both-wall move-forward config",
        .validate = validate_set_move_forward_both_wall_test,
        .execute = execute_set_move_forward_both_wall_test
    },
    {
        .name = "rotate-default",
        .help = "Use default rotate config",
        .validate = validate_set_rotate_default,
        .execute = execute_set_rotate_default
    },
    {
        .name = "rotate-test",
        .help = "Use test rotate config",
        .validate = validate_set_rotate_test,
        .execute = execute_set_rotate_test
    },
    {
        .name = "front-wall-default",
        .help = "Use default front wall config",
        .validate = validate_set_front_wall_default,
        .execute = execute_set_front_wall_default
    },
    {
        .name = "front-wall-test",
        .help = "Use test front wall config",
        .validate = validate_set_front_wall_test,
        .execute = execute_set_front_wall_test
    },
    {
        .name = "side-wall-default",
        .help = "Use default side wall config",
        .validate = validate_set_side_wall_default,
        .execute = execute_set_side_wall_default
    },
    {
        .name = "side-wall-test",
        .help = "Use test side wall config",
        .validate = validate_set_side_wall_test,
        .execute = execute_set_side_wall_test
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

    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    if (cmd->token_count == max_token_count) {
        struct maze_solver_config cfg;
        cfg.maze_size = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);

        if (cfg.maze_size > 16u) {
            cmd->bad_parameter_index = base + PARAM_0_OFFSET;
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

    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    struct maze_solver_config cfg;
    cfg.maze_size = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.total_timeout_sec = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.move_forward_time_sec = (uint32_t)strtoul(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.rotate_90_deg_time_sec = (uint32_t)strtoul(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.rotate_180_deg_time_sec = (uint32_t)strtoul(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);

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
    return validate_just_command_param_count(cmd, MOUSE_PHYSICAL_PARAMETER_COUNT);
}

void execute_set_mouse_physical_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        calculate_mouse_params(get_saved_test_mouse_physical_params());
        calculate_navigation_params();
        return;
    }

    uint32_t base = SET_COMMAND_TOKEN_COUNT;

    struct mouse_physical_params p = {0};
    p.wheel_diameter_mm = strtof(cmd->tokens[base + PARAM_0_OFFSET], NULL);
    p.wheel_base_mm = strtof(cmd->tokens[base + PARAM_1_OFFSET], NULL);
    p.max_motor_rpm = strtof(cmd->tokens[base + PARAM_2_OFFSET], NULL);
    p.encoder_events_per_revolution = strtof(cmd->tokens[base + PARAM_3_OFFSET], NULL);
    p.motor_pinion_gear_teeth = strtof(cmd->tokens[base + PARAM_4_OFFSET], NULL);
    p.wheel_gear_teeth = strtof(cmd->tokens[base + PARAM_5_OFFSET], NULL);

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
    return validate_just_command_param_count(cmd, MAZE_PHYSICAL_PARAMETER_COUNT);
}

void execute_set_maze_physical_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        calculate_maze_params(get_saved_test_maze_physical_params());
        calculate_navigation_params();
        return;
    }

    uint32_t base = SET_COMMAND_TOKEN_COUNT;

    struct maze_physical_params p = {0};
    p.post_size_mm = strtof(cmd->tokens[base + PARAM_0_OFFSET], NULL);
    p.wall_size_mm = strtof(cmd->tokens[base + PARAM_1_OFFSET], NULL);

    save_maze_physical_params_as_test(p);
    calculate_maze_params(p);
    calculate_navigation_params();
}

/*----------------------------------------------------------------------------*/
/* set move-forward-no-wall-default */
enum validation_result validate_set_move_forward_no_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_move_forward_no_wall_default(struct command const *cmd)
{
    (void)cmd;

    set_no_wall_move_forward_control_config(
        get_saved_default_move_forward_control_no_wall_config());
}

/*----------------------------------------------------------------------------*/
/* set move-forward-no-wall-test */
enum validation_result validate_set_move_forward_no_wall_test(struct command *cmd)
{
    return validate_move_forward_command(cmd);
}

void execute_set_move_forward_no_wall_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_no_wall_move_forward_control_config(
            get_saved_test_move_forward_control_no_wall_config());
        return;
    }

    struct move_forward_control_config cfg = parse_move_forward_command(cmd);

    save_move_forward_control_no_wall_config_as_test(cfg);
    set_no_wall_move_forward_control_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-one-wall-default */
enum validation_result validate_set_move_forward_one_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_move_forward_one_wall_default(struct command const *cmd)
{
    (void)cmd;

    set_one_wall_move_forward_control_config(
        get_saved_default_move_forward_control_one_wall_config());
}

/*----------------------------------------------------------------------------*/
/* set move-forward-one-wall-test */
enum validation_result validate_set_move_forward_one_wall_test(struct command *cmd)
{
    return validate_move_forward_command(cmd);
}

void execute_set_move_forward_one_wall_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_one_wall_move_forward_control_config(
            get_saved_test_move_forward_control_one_wall_config());
        return;
    }

    struct move_forward_control_config cfg = parse_move_forward_command(cmd);

    save_move_forward_control_one_wall_config_as_test(cfg);
    set_one_wall_move_forward_control_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-both-wall-default */
enum validation_result validate_set_move_forward_both_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_move_forward_both_wall_default(struct command const *cmd)
{
    (void)cmd;

    set_both_wall_move_forward_control_config(
        get_saved_default_move_forward_control_both_wall_config());
}

/*----------------------------------------------------------------------------*/
/* set move-forward-both-wall-test */
enum validation_result validate_set_move_forward_both_wall_test(struct command *cmd)
{
    return validate_move_forward_command(cmd);
}

void execute_set_move_forward_both_wall_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_both_wall_move_forward_control_config(
            get_saved_test_move_forward_control_both_wall_config());
        return;
    }

    struct move_forward_control_config cfg = parse_move_forward_command(cmd);

    save_move_forward_control_both_wall_config_as_test(cfg);
    set_both_wall_move_forward_control_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set rotate-default */
enum validation_result validate_set_rotate_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_rotate_default(struct command const *cmd)
{
    (void)cmd;

    set_rotate_control_config(get_saved_default_rotate_control_config());
}

/*----------------------------------------------------------------------------*/
/* set rotate-test */
enum validation_result validate_set_rotate_test(struct command *cmd)
{
    return validate_just_command_param_count(cmd, ROTATE_CONTROL_PARAMETER_COUNT);
}

void execute_set_rotate_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_rotate_control_config(get_saved_test_rotate_control_config());
        return;
    }

    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    struct rotate_control_config cfg = {0};
    cfg.base_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.min_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.max_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.kp_velocity = (int32_t)strtol(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.kd_velocity = (int32_t)strtol(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);
    cfg.kp_angle = (int32_t)strtol(cmd->tokens[base + PARAM_5_OFFSET], NULL, 10);
    cfg.kd_angle = (int32_t)strtol(cmd->tokens[base + PARAM_6_OFFSET], NULL, 10);
    cfg.pid_scale = (int32_t)strtol(cmd->tokens[base + PARAM_7_OFFSET], NULL, 10);

    save_rotate_control_config_as_test(cfg);
    set_rotate_control_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set front-wall-default */
enum validation_result validate_set_front_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_front_wall_default(struct command const *cmd)
{
    (void)cmd;

    set_front_wall_detection_config(get_saved_default_front_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* set front-wall-test */
enum validation_result validate_set_front_wall_test(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + FRONT_WALL_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    if (cmd->token_count == max_token_count) {
        uint32_t base = SET_COMMAND_TOKEN_COUNT;
        struct front_wall_detection_config cfg = {0};
        cfg.reading_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);

        if (cfg.reading_threshold >= 1023u) {
            cmd->bad_parameter_index = base + PARAM_0_OFFSET;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_front_wall_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_front_wall_detection_config(get_saved_test_front_wall_detection_config());
        return;
    }

    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    struct front_wall_detection_config cfg = {0};
    cfg.reading_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.num_detection_samples = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);

    save_front_wall_detection_config_as_test(cfg);
    set_front_wall_detection_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* set side-wall-default */
enum validation_result validate_set_side_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_side_wall_default(struct command const *cmd)
{
    (void)cmd;

    set_side_wall_detection_config(get_saved_default_side_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* set side-wall-test */
enum validation_result validate_set_side_wall_test(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + SIDE_WALL_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    if (cmd->token_count == max_token_count) {
        uint32_t base = SET_COMMAND_TOKEN_COUNT;
        struct side_wall_detection_config cfg = {0};
        cfg.reading_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
        cfg.slope_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
        cfg.reading_start_offset = strtof(cmd->tokens[base + PARAM_3_OFFSET], NULL);

        if (cfg.reading_threshold >= 1023u) {
            cmd->bad_parameter_index = base + PARAM_0_OFFSET;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }

        if (cfg.slope_threshold >= 1023u) {
            cmd->bad_parameter_index = base + PARAM_1_OFFSET;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }

        if ((cfg.reading_start_offset < 0.0) || (cfg.reading_start_offset >= 1.0)) {
            cmd->bad_parameter_index = base + PARAM_3_OFFSET;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_side_wall_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_side_wall_detection_config(get_saved_test_side_wall_detection_config());
        return;
    }

    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    struct side_wall_detection_config cfg = {0};
    cfg.reading_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.slope_threshold = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.num_detection_samples = (uint32_t)strtoul(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.reading_start_offset = strtof(cmd->tokens[base + PARAM_3_OFFSET], NULL);

    save_side_wall_detection_config_as_test(cfg);
    set_side_wall_detection_config(cfg);
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_just_command_param_count(struct command *cmd,
                                                                uint32_t param_count)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + param_count;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

static enum validation_result validate_move_forward_command(struct command *cmd)
{
    uint32_t max_token_count = SET_COMMAND_TOKEN_COUNT + MOVE_FORWARD_CONTROL_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != max_token_count)) {
        if (cmd->token_count < max_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    if (cmd->token_count == max_token_count) {
        uint32_t base = SET_COMMAND_TOKEN_COUNT;
        struct move_forward_control_config cfg = {0};
        cfg.wall_target = (uint32_t)strtoul(cmd->tokens[base + PARAM_10_OFFSET], NULL, 10);

        if (cfg.wall_target > 1023u) {
            cmd->bad_parameter_index = base + PARAM_10_OFFSET;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

static struct move_forward_control_config parse_move_forward_command(struct command const *cmd)
{
    uint32_t base = SET_COMMAND_TOKEN_COUNT;
    struct move_forward_control_config cfg = {0};
    cfg.base_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.min_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.max_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.kp_velocity = (int32_t)strtol(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.kd_velocity = (int32_t)strtol(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);
    cfg.kp_angle = (int32_t)strtol(cmd->tokens[base + PARAM_5_OFFSET], NULL, 10);
    cfg.kd_angle = (int32_t)strtol(cmd->tokens[base + PARAM_6_OFFSET], NULL, 10);
    cfg.kp_ir = (int32_t)strtol(cmd->tokens[base + PARAM_7_OFFSET], NULL, 10);
    cfg.kd_ir = (int32_t)strtol(cmd->tokens[base + PARAM_8_OFFSET], NULL, 10);
    cfg.pid_scale = (int32_t)strtol(cmd->tokens[base + PARAM_9_OFFSET], NULL, 10);
    cfg.wall_target = (uint32_t)strtoul(cmd->tokens[base + PARAM_10_OFFSET], NULL, 10);

    return cfg;
}
