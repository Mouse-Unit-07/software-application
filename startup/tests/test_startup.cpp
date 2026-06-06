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

void init_maze_solver_common(void)
{
    mock().actualCall("init_maze_solver_common");
}

void deinit_maze_solver_common(void)
{
    mock().actualCall("deinit_maze_solver_common");
}

struct maze_solver_config get_default_maze_solver_config(void)
{
    mock().actualCall("get_default_maze_solver_config");

    struct maze_solver_config cfg{};
    return cfg;
}

void set_maze_solver_config(struct maze_solver_config cfg)
{
    (void)cfg;

    mock().actualCall("set_maze_solver_config");
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

    mock().expectOneCall("init_maze_solver_common");
    mock().expectOneCall("get_default_maze_solver_config");
    mock().expectOneCall("set_maze_solver_config");

    startup();
}

TEST(StartupTests, ShutdownCallsFunctions)
{
    mock().expectOneCall("deinit_maze_solver_common");

    mock().expectOneCall("deinit_navigation");
    mock().expectOneCall("deinit_user_interface");
    mock().expectOneCall("deinit_device_self_tests");
    mock().expectOneCall("deinit_fault_detector");
    mock().expectOneCall("deinit_global_time");
    mock().expectOneCall("deinit_mouse");

    shutdown();
}