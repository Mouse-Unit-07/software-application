/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : startup.c                                             */
/*                                                                            */
/* Implementation for micromouse startup library                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include "device_self_tests.h"
#include "fault_detector.h"
#include "global_time.h"
#include "initialization.h"
#include "navigation.h"
#include "user_interface.h"
#include "maze_solver_common.h"
#include "configuration.h"
#include "startup.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void startup(void)
{
    init_mouse();
    init_global_time();
    init_fault_detector();
    init_device_self_tests();
    init_user_interface();
    init_navigation();

    init_maze_solver_common();
    set_maze_solver_config(get_default_maze_solver_config());
}

void shutdown(void)
{
    deinit_maze_solver_common();

    deinit_navigation();
    deinit_user_interface();
    deinit_device_self_tests();
    deinit_fault_detector();
    deinit_global_time();
    deinit_mouse();
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
