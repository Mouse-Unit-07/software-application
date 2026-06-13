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
#include "navigation.h"
#include "configuration.h"
#include "command.h"
#include "get.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void print_maze_solver_config(struct maze_solver_config cfg);
static void print_mouse_physical_params(struct mouse_physical_params p);
static void print_mouse_calculated_params(struct mouse_calculated_params p);
static void print_maze_physical_params(struct maze_physical_params p);
static void print_maze_calculated_params(struct maze_calculated_params p);
static void print_navigation_params(struct navigation_params p);
static void print_move_forward_control_config(struct move_forward_control_config cfg);
static void print_rotate_control_config(struct rotate_control_config cfg);
static void print_front_wall_detection_config(struct front_wall_detection_config cfg);
static void print_side_wall_detection_config(struct side_wall_detection_config cfg);
static void print_side_wall_calculated_params(struct side_wall_calculated_params p);

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
    },
    {
        .name = "mouse-physical-default",
        .help = "Display default mouse physical parameters",
        .validate = validate_get_mouse_physical_default,
        .execute = execute_get_mouse_physical_default
    },
    {
        .name = "mouse-physical-test",
        .help = "Display test mouse physical parameters",
        .validate = validate_get_mouse_physical_test,
        .execute = execute_get_mouse_physical_test
    },
    {
        .name = "mouse-physical-current",
        .help = "Display current mouse physical parameters",
        .validate = validate_get_mouse_physical_current,
        .execute = execute_get_mouse_physical_current
    },
    {
        .name = "mouse-calculated",
        .help = "Display mouse calculated parameters",
        .validate = validate_get_mouse_calculated,
        .execute = execute_get_mouse_calculated
    },
    {
        .name = "maze-physical-default",
        .help = "Display default maze physical parameters",
        .validate = validate_get_maze_physical_default,
        .execute = execute_get_maze_physical_default
    },
    {
        .name = "maze-physical-test",
        .help = "Display test maze physical parameters",
        .validate = validate_get_maze_physical_test,
        .execute = execute_get_maze_physical_test
    },
    {
        .name = "maze-physical-current",
        .help = "Display current maze physical parameters",
        .validate = validate_get_maze_physical_current,
        .execute = execute_get_maze_physical_current
    },
    {
        .name = "maze-calculated",
        .help = "Display maze calculated parameters",
        .validate = validate_get_maze_calculated,
        .execute = execute_get_maze_calculated
    },
    {
        .name = "navigation",
        .help = "Display navigation parameters",
        .validate = validate_get_navigation,
        .execute = execute_get_navigation
    },
    {
        .name = "move-forward-no-wall-default",
        .help = "Display default no-wall move-forward control config",
        .validate = validate_get_move_forward_no_wall_default,
        .execute = execute_get_move_forward_no_wall_default
    },
    {
        .name = "move-forward-no-wall-test",
        .help = "Display test no-wall move-forward control config",
        .validate = validate_get_move_forward_no_wall_test,
        .execute = execute_get_move_forward_no_wall_test
    },
    {
        .name = "move-forward-no-wall-current",
        .help = "Display current no-wall move-forward control config",
        .validate = validate_get_move_forward_no_wall_current,
        .execute = execute_get_move_forward_no_wall_current
    },
    {
        .name = "move-forward-one-wall-default",
        .help = "Display default one-wall move-forward control config",
        .validate = validate_get_move_forward_one_wall_default,
        .execute = execute_get_move_forward_one_wall_default
    },
    {
        .name = "move-forward-one-wall-test",
        .help = "Display test one-wall move-forward control config",
        .validate = validate_get_move_forward_one_wall_test,
        .execute = execute_get_move_forward_one_wall_test
    },
    {
        .name = "move-forward-one-wall-current",
        .help = "Display current one-wall move-forward control config",
        .validate = validate_get_move_forward_one_wall_current,
        .execute = execute_get_move_forward_one_wall_current
    },
    {
        .name = "move-forward-both-wall-default",
        .help = "Display default both-wall move-forward control config",
        .validate = validate_get_move_forward_both_wall_default,
        .execute = execute_get_move_forward_both_wall_default
    },
    {
        .name = "move-forward-both-wall-test",
        .help = "Display test both-wall move-forward control config",
        .validate = validate_get_move_forward_both_wall_test,
        .execute = execute_get_move_forward_both_wall_test
    },
    {
        .name = "move-forward-both-wall-current",
        .help = "Display current both-wall move-forward control config",
        .validate = validate_get_move_forward_both_wall_current,
        .execute = execute_get_move_forward_both_wall_current
    },
    {
        .name = "rotate-default",
        .help = "Display default rotation control config",
        .validate = validate_get_rotate_default,
        .execute = execute_get_rotate_default
    },
    {
        .name = "rotate-test",
        .help = "Display test rotation control config",
        .validate = validate_get_rotate_test,
        .execute = execute_get_rotate_test
    },
    {
        .name = "rotate-current",
        .help = "Display current rotation control config",
        .validate = validate_get_rotate_current,
        .execute = execute_get_rotate_current
    },
    {
        .name = "front-wall-default",
        .help = "Display default front-wall detection control config",
        .validate = validate_get_front_wall_default,
        .execute = execute_get_front_wall_default
    },
    {
        .name = "front-wall-test",
        .help = "Display test front-wall control config",
        .validate = validate_get_front_wall_test,
        .execute = execute_get_front_wall_test
    },
    {
        .name = "front-wall-current",
        .help = "Display current front-wall control config",
        .validate = validate_get_front_wall_current,
        .execute = execute_get_front_wall_current
    },
    {
        .name = "side-wall-default",
        .help = "Display default side-wall detection control config",
        .validate = validate_get_side_wall_default,
        .execute = execute_get_side_wall_default
    },
    {
        .name = "side-wall-test",
        .help = "Display test side-wall control config",
        .validate = validate_get_side_wall_test,
        .execute = execute_get_side_wall_test
    },
    {
        .name = "side-wall-current",
        .help = "Display current side-wall control config",
        .validate = validate_get_side_wall_current,
        .execute = execute_get_side_wall_current
    },
    {
        .name = "side-wall-calculated",
        .help = "Display side-wall calculated parameters",
        .validate = validate_get_side_wall_calculated,
        .execute = execute_get_side_wall_calculated
    },
};

static const struct command_node get_node =
{
    .name = "get",
    .help = "Read configuration values",
    .validate = validate_get,
    .execute = execute_get,
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
    (void)cmd;

    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_get(struct command const *cmd)
{
    (void)cmd;

    printf("get command parameters:\r\n");
    print_command_help(get_commands, get_get_commands_count());
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

    print_maze_solver_config(get_saved_default_maze_solver_config());
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

    print_maze_solver_config(get_saved_test_maze_solver_config());
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
/* get mouse-physical-default */
enum validation_result validate_get_mouse_physical_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_mouse_physical_default(struct command const *cmd)
{
    (void)cmd;

    print_mouse_physical_params(get_saved_default_mouse_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-test */
enum validation_result validate_get_mouse_physical_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_mouse_physical_test(struct command const *cmd)
{
    (void)cmd;

    print_mouse_physical_params(get_saved_test_mouse_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-current */
enum validation_result validate_get_mouse_physical_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_mouse_physical_current(struct command const *cmd)
{
    (void)cmd;

    print_mouse_physical_params(get_mouse_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get mouse-calculated */
enum validation_result validate_get_mouse_calculated(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_mouse_calculated(struct command const *cmd)
{
    (void)cmd;

    print_mouse_calculated_params(get_mouse_calculated_params());
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-default */
enum validation_result validate_get_maze_physical_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_maze_physical_default(struct command const *cmd)
{
    (void)cmd;

    print_maze_physical_params(get_saved_default_maze_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-test */
enum validation_result validate_get_maze_physical_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_maze_physical_test(struct command const *cmd)
{
    (void)cmd;

    print_maze_physical_params(get_saved_test_maze_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-current */
enum validation_result validate_get_maze_physical_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_maze_physical_current(struct command const *cmd)
{
    (void)cmd;

    print_maze_physical_params(get_maze_physical_params());
}

/*----------------------------------------------------------------------------*/
/* get maze-calculated */
enum validation_result validate_get_maze_calculated(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_maze_calculated(struct command const *cmd)
{
    (void)cmd;

    print_maze_calculated_params(get_maze_calculated_params());
}

/*----------------------------------------------------------------------------*/
/* get navigation */
enum validation_result validate_get_navigation(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_navigation(struct command const *cmd)
{
    (void)cmd;

    print_navigation_params(get_navigation_params());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-default */
enum validation_result validate_get_move_forward_no_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_no_wall_default(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_default_move_forward_control_no_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-test */
enum validation_result validate_get_move_forward_no_wall_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_no_wall_test(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_test_move_forward_control_no_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-current */
enum validation_result validate_get_move_forward_no_wall_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_no_wall_current(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_no_wall_move_forward_control_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-default */
enum validation_result validate_get_move_forward_one_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_one_wall_default(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_default_move_forward_control_one_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-test */
enum validation_result validate_get_move_forward_one_wall_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_one_wall_test(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_test_move_forward_control_one_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-current */
enum validation_result validate_get_move_forward_one_wall_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_one_wall_current(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_one_wall_move_forward_control_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-default */
enum validation_result validate_get_move_forward_both_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_both_wall_default(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_default_move_forward_control_both_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-test */
enum validation_result validate_get_move_forward_both_wall_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_both_wall_test(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_saved_test_move_forward_control_both_wall_config());
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-current */
enum validation_result validate_get_move_forward_both_wall_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_move_forward_both_wall_current(struct command const *cmd)
{
    (void)cmd;

    print_move_forward_control_config(get_both_wall_move_forward_control_config());
}

/*----------------------------------------------------------------------------*/
/* get rotate-default */
enum validation_result validate_get_rotate_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_rotate_default(struct command const *cmd)
{
    (void)cmd;

    print_rotate_control_config(get_saved_default_rotate_control_config());
}

/*----------------------------------------------------------------------------*/
/* get rotate-test */
enum validation_result validate_get_rotate_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_rotate_test(struct command const *cmd)
{
    (void)cmd;

    print_rotate_control_config(get_saved_test_rotate_control_config());
}

/*----------------------------------------------------------------------------*/
/* get rotate-current */
enum validation_result validate_get_rotate_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_rotate_current(struct command const *cmd)
{
    (void)cmd;

    print_rotate_control_config(get_rotate_control_config());
}
/*----------------------------------------------------------------------------*/
/* get front-wall-default */
enum validation_result validate_get_front_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_front_wall_default(struct command const *cmd)
{
    (void)cmd;

    print_front_wall_detection_config(get_saved_default_front_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get front-wall-test */
enum validation_result validate_get_front_wall_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_front_wall_test(struct command const *cmd)
{
    (void)cmd;

    print_front_wall_detection_config(get_saved_test_front_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get front-wall-current */
enum validation_result validate_get_front_wall_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_front_wall_current(struct command const *cmd)
{
    (void)cmd;

    print_front_wall_detection_config(get_front_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get side-wall-default */
enum validation_result validate_get_side_wall_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_side_wall_default(struct command const *cmd)
{
    (void)cmd;

    print_side_wall_detection_config(get_saved_default_side_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get side-wall-test */
enum validation_result validate_get_side_wall_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_side_wall_test(struct command const *cmd)
{
    (void)cmd;

    print_side_wall_detection_config(get_saved_test_side_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get side-wall-current */
enum validation_result validate_get_side_wall_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_side_wall_current(struct command const *cmd)
{
    (void)cmd;

    print_side_wall_detection_config(get_side_wall_detection_config());
}

/*----------------------------------------------------------------------------*/
/* get side-wall-calculated */
enum validation_result validate_get_side_wall_calculated(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_side_wall_calculated(struct command const *cmd)
{
    (void)cmd;

    print_side_wall_calculated_params(get_side_wall_calculated_params());
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

static void print_mouse_physical_params(struct mouse_physical_params p)
{
    printf("wheel_diameter_mm                = %.3f\r\n", p.wheel_diameter_mm);
    printf("wheel_base_mm                    = %.3f\r\n", p.wheel_base_mm);
    printf("max_motor_rpm                    = %.3f\r\n", p.max_motor_rpm);
    printf("encoder_events_per_revolution    = %.3f\r\n", p.encoder_events_per_revolution);
    printf("motor_pinion_gear_teeth          = %.3f\r\n", p.motor_pinion_gear_teeth);
    printf("wheel_gear_teeth                 = %.3f\r\n", p.wheel_gear_teeth);
}

static void print_mouse_calculated_params(struct mouse_calculated_params p)
{
    printf("gear_ratio                       = %.6f\r\n", p.gear_ratio);
    printf("encoder_ticks_per_revolution     = %.6f\r\n", p.encoder_ticks_per_revolution);
    printf("encoder_ticks_per_millimeter     = %.6f\r\n", p.encoder_ticks_per_millimeter);
    printf("encoder_ticks_per_rotation_rad   = %.6f\r\n", p.encoder_ticks_per_rotation_radian);
}

static void print_maze_physical_params(struct maze_physical_params p)
{
    printf("post_size_mm                     = %.3f\r\n", p.post_size_mm);
    printf("wall_size_mm                     = %.3f\r\n", p.wall_size_mm);
}

static void print_maze_calculated_params(struct maze_calculated_params p)
{
    printf("cell_size_mm                     = %.3f\r\n", p.cell_size_mm);
}

static void print_navigation_params(struct navigation_params p)
{
    printf("move_forward_one_cell_ticks      = %" PRId32 "\r\n",
           p.move_forward_one_cell_target_ticks);
    printf("rotate_90_degree_ticks           = %" PRId32 "\r\n", p.rotate_90_degree_target_ticks);
    printf("rotate_180_degree_ticks          = %" PRId32 "\r\n", p.rotate_180_degree_target_ticks);
}

static void print_move_forward_control_config(struct move_forward_control_config cfg)
{
    printf("base_speed     = %" PRIu8 "\r\n", cfg.base_speed);
    printf("min_speed      = %" PRIu8 "\r\n", cfg.min_speed);
    printf("max_speed      = %" PRIu8 "\r\n", cfg.max_speed);
    printf("kp_velocity    = %" PRId32 "\r\n", cfg.kp_velocity);
    printf("kd_velocity    = %" PRId32 "\r\n", cfg.kd_velocity);
    printf("kp_angle       = %" PRId32 "\r\n", cfg.kp_angle);
    printf("kd_angle       = %" PRId32 "\r\n", cfg.kd_angle);
    printf("kp_ir          = %" PRId32 "\r\n", cfg.kp_ir);
    printf("kd_ir          = %" PRId32 "\r\n", cfg.kd_ir);
    printf("pid_scale      = %" PRId32 "\r\n", cfg.pid_scale);
    printf("wall_target    = %" PRIu32 "\r\n", cfg.wall_target);
}

static void print_rotate_control_config(struct rotate_control_config cfg)
{
    printf("base_speed     = %" PRIu8 "\r\n", cfg.base_speed);
    printf("min_speed      = %" PRIu8 "\r\n", cfg.min_speed);
    printf("max_speed      = %" PRIu8 "\r\n", cfg.max_speed);
    printf("kp_velocity    = %" PRId32 "\r\n", cfg.kp_velocity);
    printf("kd_velocity    = %" PRId32 "\r\n", cfg.kd_velocity);
    printf("kp_angle       = %" PRId32 "\r\n", cfg.kp_angle);
    printf("kd_angle       = %" PRId32 "\r\n", cfg.kd_angle);
    printf("pid_scale      = %" PRId32 "\r\n", cfg.pid_scale);
}

static void print_front_wall_detection_config(struct front_wall_detection_config cfg)
{
    printf("reading_threshold        = %" PRIu32 "\r\n", cfg.reading_threshold);
    printf("num_detection_samples    = %" PRIu32 "\r\n", cfg.num_detection_samples);
}

static void print_side_wall_detection_config(struct side_wall_detection_config cfg)
{
    printf("reading_threshold        = %" PRIu32 "\r\n", cfg.reading_threshold);
    printf("slope_threshold          = %" PRIu32 "\r\n", cfg.slope_threshold);
    printf("num_detection_samples    = %" PRIu32 "\r\n", cfg.num_detection_samples);
    printf("reading_start_offset     = %.3f\r\n", cfg.reading_start_offset);
}

static void print_side_wall_calculated_params(struct side_wall_calculated_params p)
{
    printf("reading_start_offset_ticks    = %" PRIu32 "\r\n", p.reading_start_offset_ticks);
}
