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
/* none */

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

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void init_configuration(void)
{
    memcpy(&test_maze_solver_config, &default_maze_solver_config, sizeof(test_maze_solver_config));
    memcpy(&test_mouse_physical_params, &default_mouse_physical_params,
           sizeof(test_mouse_physical_params));
    memcpy(&test_maze_physical_params, &default_maze_physical_params,
           sizeof(test_maze_physical_params));
}

void deinit_configuration(void)
{
    memcpy(&test_maze_solver_config, &default_maze_solver_config, sizeof(test_maze_solver_config));
    memcpy(&test_mouse_physical_params, &default_mouse_physical_params,
           sizeof(test_mouse_physical_params));
    memcpy(&test_maze_physical_params, &default_maze_physical_params,
           sizeof(test_maze_physical_params));
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

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
