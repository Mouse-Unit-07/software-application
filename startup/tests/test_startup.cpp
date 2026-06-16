/*================================ FILE INFO =================================*/
/* Filename           : test_startup.cpp                                      */
/*                                                                            */
/* Test implementation for startup.c                                          */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdbool.h>
#include <stdint.h>
#include "maze_solver_common.h"
#include "navigation.h"
#include "startup.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
/* none */

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

void init_mouse(void)
{
    mock().actualCall("init_mouse");
}

void deinit_mouse(void)
{
    mock().actualCall("deinit_mouse");
}

void init_global_time(void)
{
    mock().actualCall("init_global_time");
}

void deinit_global_time(void)
{
    mock().actualCall("deinit_global_time");
}

void init_fault_detector(void)
{
    mock().actualCall("init_fault_detector");
}

void deinit_fault_detector(void)
{
    mock().actualCall("deinit_fault_detector");
}

void init_device_self_tests(void)
{
    mock().actualCall("init_device_self_tests");
}

void deinit_device_self_tests(void)
{
    mock().actualCall("deinit_device_self_tests");
}

void init_user_interface(void)
{
    mock().actualCall("init_user_interface");
}

void deinit_user_interface(void)
{
    mock().actualCall("deinit_user_interface");
}

void init_navigation(void)
{
    mock().actualCall("init_navigation");
}

void deinit_navigation(void)
{
    mock().actualCall("deinit_navigation");
}

void init_configuration(void)
{
    mock().actualCall("init_configuration");
}

void deinit_configuration(void)
{
    mock().actualCall("deinit_configuration");
}

void init_maze_solver_common(void)
{
    mock().actualCall("init_maze_solver_common");
}

void deinit_maze_solver_common(void)
{
    mock().actualCall("deinit_maze_solver_common");
}

struct maze_solver_config get_saved_default_maze_solver_config(void)
{
    mock().actualCall("get_saved_default_maze_solver_config");

    struct maze_solver_config cfg{};
    return cfg;
}

void set_maze_solver_config(struct maze_solver_config cfg)
{
    (void)cfg;

    mock().actualCall("set_maze_solver_config");
}

struct mouse_physical_params get_saved_default_mouse_physical_params(void)
{
    mock().actualCall("get_saved_default_mouse_physical_params");

    struct mouse_physical_params cfg{};
    return cfg;
}

struct maze_physical_params get_saved_default_maze_physical_params(void)
{
    mock().actualCall("get_saved_default_maze_physical_params");

    struct maze_physical_params cfg{};
    return cfg;
}

void calculate_mouse_params(struct mouse_physical_params p)
{
    (void)p;

    mock().actualCall("calculate_mouse_params");
}

void calculate_maze_params(struct maze_physical_params p)
{
    (void)p;

    mock().actualCall("calculate_maze_params");
}

void calculate_navigation_params(void)
{
    mock().actualCall("calculate_navigation_params");
}

void set_no_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    (void)cfg;

    mock().actualCall("set_no_wall_move_forward_control_config");
}

void set_one_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    (void)cfg;

    mock().actualCall("set_one_wall_move_forward_control_config");
}

void set_both_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    (void)cfg;

    mock().actualCall("set_both_wall_move_forward_control_config");
}

void set_rotate_control_config(struct rotate_control_config cfg)
{
    (void)cfg;

    mock().actualCall("set_rotate_control_config");
}

void set_side_wall_detection_config(struct side_wall_detection_config cfg)
{
    (void)cfg;

    mock().actualCall("set_side_wall_detection_config");
}

void set_front_wall_detection_config(struct front_wall_detection_config cfg)
{
    (void)cfg;

    mock().actualCall("set_front_wall_detection_config");

}

struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_no_wall_config");

    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_one_wall_config");

    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_both_wall_config");

    struct move_forward_control_config cfg{};
    return cfg;
}

struct rotate_control_config get_saved_default_rotate_control_config(void)
{
    mock().actualCall("get_saved_default_rotate_control_config");

    struct rotate_control_config cfg{};
    return cfg;
}

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void)
{
    mock().actualCall("get_saved_default_front_wall_detection_config");

    struct front_wall_detection_config cfg{};
    return cfg;
}

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void)
{
    mock().actualCall("get_saved_default_side_wall_detection_config");

    struct side_wall_detection_config cfg{};
    return cfg;
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(StartupTests)
{
    void setup() override
    {
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(StartupTests, StartupCallsFunctions)
{
    mock().expectOneCall("init_mouse");
    mock().expectOneCall("init_global_time");
    mock().expectOneCall("init_fault_detector");
    mock().expectOneCall("init_device_self_tests");
    mock().expectOneCall("init_user_interface");
    mock().expectOneCall("init_navigation");
    mock().expectOneCall("get_saved_default_mouse_physical_params");
    mock().expectOneCall("calculate_mouse_params");
    mock().expectOneCall("get_saved_default_maze_physical_params");
    mock().expectOneCall("calculate_maze_params");
    mock().expectOneCall("calculate_navigation_params");
    mock().expectOneCall("get_saved_default_move_forward_control_no_wall_config");
    mock().expectOneCall("set_no_wall_move_forward_control_config");
    mock().expectOneCall("get_saved_default_move_forward_control_one_wall_config");
    mock().expectOneCall("set_one_wall_move_forward_control_config");
    mock().expectOneCall("get_saved_default_move_forward_control_both_wall_config");
    mock().expectOneCall("set_both_wall_move_forward_control_config");
    mock().expectOneCall("get_saved_default_rotate_control_config");
    mock().expectOneCall("set_rotate_control_config");
    mock().expectOneCall("get_saved_default_side_wall_detection_config");
    mock().expectOneCall("set_side_wall_detection_config");
    mock().expectOneCall("get_saved_default_front_wall_detection_config");
    mock().expectOneCall("set_front_wall_detection_config");

    mock().expectOneCall("init_configuration");

    mock().expectOneCall("init_maze_solver_common");
    mock().expectOneCall("get_saved_default_maze_solver_config");
    mock().expectOneCall("set_maze_solver_config");

    startup();
}

TEST(StartupTests, ShutdownCallsFunctions)
{
    mock().expectOneCall("deinit_maze_solver_common");

    mock().expectOneCall("deinit_configuration");

    mock().expectOneCall("deinit_navigation");
    mock().expectOneCall("deinit_user_interface");
    mock().expectOneCall("deinit_device_self_tests");
    mock().expectOneCall("deinit_fault_detector");
    mock().expectOneCall("deinit_global_time");
    mock().expectOneCall("deinit_mouse");

    shutdown();
}
