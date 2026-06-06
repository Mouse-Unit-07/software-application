/*================================ FILE INFO =================================*/
/* Filename           : test_configuration.cpp                                */
/*                                                                            */
/* Test implementation for configuration.c                                    */
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
#include "configuration.h"

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

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(ConfigurationTests)
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
TEST(ConfigurationTests, DefaultMazeSolverConfigContainsExpectedValues)
{
    struct maze_solver_config cfg{get_default_maze_solver_config()};

    LONGS_EQUAL(16u, cfg.maze_size);
    LONGS_EQUAL(600u, cfg.total_timeout_sec);
    LONGS_EQUAL(1u, cfg.move_forward_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_90_deg_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_180_deg_time_sec);
}
