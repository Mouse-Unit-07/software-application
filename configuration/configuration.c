/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : configuration.c                                       */
/*                                                                            */
/* Implementation for micromouse configuration library                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "maze_solver_common.h"
#include "navigation.h"
#include "configuration.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void reset_all_test_configs(void);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
static const struct maze_solver_config default_maze_solver_config =
{
    .maze_size = 16u,
    .total_timeout_sec = 10u * 60u,
    .move_forward_time_sec = 1u,
    .rotate_90_deg_time_sec = 1u,
    .rotate_180_deg_time_sec = 1u
};

static struct maze_solver_config test_maze_solver_config = {0};

static const struct mouse_physical_params default_mouse_physical_params =
{
    .wheel_diameter_mm = 32.0,
    .wheel_base_mm = 87.56,
    .max_motor_rpm = 4900.0,
    .encoder_events_per_revolution = 60.8077,
    .motor_pinion_gear_teeth = 13.0,
    .wheel_gear_teeth = 44.0
};

static struct mouse_physical_params test_mouse_physical_params = {0};

static const struct maze_physical_params default_maze_physical_params =
{
    .post_size_mm = 12.07,
    .wall_size_mm = 166.37
};

static struct maze_physical_params test_maze_physical_params = {0};

static const struct move_forward_common_config default_move_forward_common_config =
{
    .emergency_stop_threshold = 400
};

static struct move_forward_common_config test_move_forward_common_config = {0};

static const struct move_forward_control_config default_move_forward_control_no_wall_config =
{
    .base_speed = 140,
    .min_speed = 140,
    .max_speed = 255,
    .kp_velocity = 783,
    .kd_velocity = 510,
    .kp_angle = 75826,
    .kd_angle = 280,
    .kp_ir = 0,
    .kd_ir = 0,
    .pid_scale = 2284,
    .wall_target = 0
};

static struct move_forward_control_config test_move_forward_control_no_wall_config = {0};

static const struct move_forward_control_config default_move_forward_control_one_wall_config =
{
    .base_speed = 142,
    .min_speed = 140,
    .max_speed = 255,
    .kp_velocity = 1133,
    .kd_velocity = 1704,
    .kp_angle = 2568,
    .kd_angle = 1796,
    .kp_ir = 1506,
    .kd_ir = 446,
    .pid_scale = 614,
    .wall_target = 164
};

static struct move_forward_control_config test_move_forward_control_one_wall_config = {0};

static const struct move_forward_control_config default_move_forward_control_both_wall_config =
{
    .base_speed = 159,
    .min_speed = 140,
    .max_speed = 255,
    .kp_velocity = 777,
    .kd_velocity = 568,
    .kp_angle = 972,
    .kd_angle = 1088,
    .kp_ir = 1967,
    .kd_ir = 671,
    .pid_scale = 2396,
    .wall_target = 0
};

static struct move_forward_control_config test_move_forward_control_both_wall_config = {0};

static const struct rotate_control_config default_rotate_control_config =
{
    .base_speed = 143,
    .min_speed = 140,
    .max_speed = 255,
    .kp_velocity = 818,
    .kd_velocity = 1256,
    .kp_angle = 34644,
    .kd_angle = 48,
    .pid_scale = 1656
};

static struct rotate_control_config test_rotate_control_config = {0};

static const struct front_wall_detection_config default_front_wall_detection_config =
{
    .reading_threshold = 161,
    .num_detection_samples = 10
};

static struct front_wall_detection_config test_front_wall_detection_config = {0};

static const struct side_wall_detection_config default_side_wall_detection_config =
{
    .reading_threshold = 49,
    .slope_threshold = 77,
    .num_detection_samples = 10,
    .reading_start_offset = 0
};

static struct side_wall_detection_config test_side_wall_detection_config = {0};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void init_configuration(void)
{
    reset_all_test_configs();
}

void deinit_configuration(void)
{
    reset_all_test_configs();
}

struct maze_solver_config get_saved_default_maze_solver_config(void)
{
    return default_maze_solver_config;
}

struct maze_solver_config get_saved_test_maze_solver_config(void)
{
    return test_maze_solver_config;
}

void save_maze_solver_config_as_test(struct maze_solver_config cfg)
{
    test_maze_solver_config = cfg;
}

struct mouse_physical_params get_saved_default_mouse_physical_params(void)
{
    return default_mouse_physical_params;
}

struct mouse_physical_params get_saved_test_mouse_physical_params(void)
{
    return test_mouse_physical_params;
}

void save_mouse_physical_params_as_test(struct mouse_physical_params cfg)
{
    test_mouse_physical_params = cfg;
}

struct maze_physical_params get_saved_default_maze_physical_params(void)
{
    return default_maze_physical_params;
}

struct maze_physical_params get_saved_test_maze_physical_params(void)
{
    return test_maze_physical_params;
}

void save_maze_physical_params_as_test(struct maze_physical_params cfg)
{
    test_maze_physical_params = cfg;
}

struct move_forward_common_config get_saved_default_move_forward_common_config(void)
{
    return default_move_forward_common_config;
}

struct move_forward_common_config get_saved_test_move_forward_common_config(void)
{
    return test_move_forward_common_config;
}

void save_move_forward_common_config_as_test(struct move_forward_common_config cfg)
{
    test_move_forward_common_config = cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void)
{
    return default_move_forward_control_no_wall_config;
}

struct move_forward_control_config get_saved_test_move_forward_control_no_wall_config(void)
{
    return test_move_forward_control_no_wall_config;
}

void save_move_forward_control_no_wall_config_as_test(struct move_forward_control_config cfg)
{
    test_move_forward_control_no_wall_config = cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void)
{
    return default_move_forward_control_one_wall_config;
}

struct move_forward_control_config get_saved_test_move_forward_control_one_wall_config(void)
{
    return test_move_forward_control_one_wall_config;
}

void save_move_forward_control_one_wall_config_as_test(struct move_forward_control_config cfg)
{
    test_move_forward_control_one_wall_config = cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void)
{
    return default_move_forward_control_both_wall_config;
}

struct move_forward_control_config get_saved_test_move_forward_control_both_wall_config(void)
{
    return test_move_forward_control_both_wall_config;
}

void save_move_forward_control_both_wall_config_as_test(struct move_forward_control_config cfg)
{
    test_move_forward_control_both_wall_config = cfg;
}

struct rotate_control_config get_saved_default_rotate_control_config(void)
{
    return default_rotate_control_config;
}

struct rotate_control_config get_saved_test_rotate_control_config(void)
{
    return test_rotate_control_config;
}

void save_rotate_control_config_as_test(struct rotate_control_config cfg)
{
    test_rotate_control_config = cfg;
}

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void)
{
    return default_front_wall_detection_config;
}

struct front_wall_detection_config get_saved_test_front_wall_detection_config(void)
{
    return test_front_wall_detection_config;
}

void save_front_wall_detection_config_as_test(struct front_wall_detection_config cfg)
{
    test_front_wall_detection_config = cfg;
}

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void)
{
    return default_side_wall_detection_config;
}

struct side_wall_detection_config get_saved_test_side_wall_detection_config(void)
{
    return test_side_wall_detection_config;
}

void save_side_wall_detection_config_as_test(struct side_wall_detection_config cfg)
{
    test_side_wall_detection_config = cfg;
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static void reset_all_test_configs(void)
{
    memcpy(&test_maze_solver_config, &default_maze_solver_config, sizeof(test_maze_solver_config));
    memcpy(&test_mouse_physical_params, &default_mouse_physical_params,
           sizeof(test_mouse_physical_params));
    memcpy(&test_maze_physical_params, &default_maze_physical_params,
           sizeof(test_maze_physical_params));
    memcpy(&test_move_forward_common_config, &default_move_forward_common_config,
           sizeof(test_move_forward_common_config));
    memcpy(&test_move_forward_control_no_wall_config, &default_move_forward_control_no_wall_config,
           sizeof(test_move_forward_control_no_wall_config));
    memcpy(&test_move_forward_control_one_wall_config,
           &default_move_forward_control_one_wall_config,
           sizeof(test_move_forward_control_one_wall_config));
    memcpy(&test_move_forward_control_both_wall_config,
           &default_move_forward_control_both_wall_config,
           sizeof(test_move_forward_control_both_wall_config));
    memcpy(&test_rotate_control_config, &default_rotate_control_config,
           sizeof(test_rotate_control_config));
    memcpy(&test_front_wall_detection_config, &default_front_wall_detection_config,
           sizeof(test_front_wall_detection_config));
    memcpy(&test_side_wall_detection_config, &default_side_wall_detection_config,
           sizeof(test_side_wall_detection_config));
}
