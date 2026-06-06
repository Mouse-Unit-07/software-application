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

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void init_configuration(void)
{
    memcpy(&test_maze_solver_config, &default_maze_solver_config, sizeof(test_maze_solver_config));
}

void deinit_configuration(void)
{
    memcpy(&test_maze_solver_config, &default_maze_solver_config, sizeof(test_maze_solver_config));
}

struct maze_solver_config get_default_maze_solver_config(void)
{
    return default_maze_solver_config;
}

struct maze_solver_config get_test_maze_solver_config(void)
{
    return test_maze_solver_config;
}

void set_test_maze_solver_config(struct maze_solver_config cfg)
{
    test_maze_solver_config = cfg;
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
