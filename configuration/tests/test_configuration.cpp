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
static void check_default_maze_solver_config(struct maze_solver_config const &cfg)
{
    LONGS_EQUAL(16u, cfg.maze_size);
    LONGS_EQUAL(600u, cfg.total_timeout_sec);
    LONGS_EQUAL(1u, cfg.move_forward_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_90_deg_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_180_deg_time_sec);
}

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
        init_configuration();
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
        deinit_configuration();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(ConfigurationTests, InitConfigurationResetsTestConfigToDefaults)
{
    struct maze_solver_config cfg{0};
    cfg.maze_size = 8u;
    cfg.total_timeout_sec = 123u;
    cfg.move_forward_time_sec = 2u;
    cfg.rotate_90_deg_time_sec = 3u;
    cfg.rotate_180_deg_time_sec = 4u;

    set_test_maze_solver_config(cfg);

    init_configuration();

    struct maze_solver_config result{get_test_maze_solver_config()};

    check_default_maze_solver_config(result);
}

TEST(ConfigurationTests, DeinitConfigurationResetsTestConfigToDefaults)
{
    struct maze_solver_config cfg{0};
    cfg.maze_size = 8u;
    cfg.total_timeout_sec = 123u;
    cfg.move_forward_time_sec = 2u;
    cfg.rotate_90_deg_time_sec = 3u;
    cfg.rotate_180_deg_time_sec = 4u;

    set_test_maze_solver_config(cfg);

    deinit_configuration();

    struct maze_solver_config result{get_test_maze_solver_config()};

    check_default_maze_solver_config(result);
}

TEST(ConfigurationTests, DefaultMazeSolverConfigContainsExpectedValues)
{
    struct maze_solver_config cfg{get_default_maze_solver_config()};

    check_default_maze_solver_config(cfg);
}

TEST(ConfigurationTests, TestMazeSolverConfigInitiallyContainsExpectedValues)
{
    struct maze_solver_config cfg{get_test_maze_solver_config()};

    check_default_maze_solver_config(cfg);
}

TEST(ConfigurationTests, SetTestMazeSolverConfigUpdatesValues)
{
    struct maze_solver_config cfg{0};
    cfg.maze_size = 8u;
    cfg.total_timeout_sec = 123u;
    cfg.move_forward_time_sec = 2u;
    cfg.rotate_90_deg_time_sec = 3u;
    cfg.rotate_180_deg_time_sec = 4u;

    set_test_maze_solver_config(cfg);

    struct maze_solver_config result{get_test_maze_solver_config()};

    LONGS_EQUAL(8u, result.maze_size);
    LONGS_EQUAL(123u, result.total_timeout_sec);
    LONGS_EQUAL(2u, result.move_forward_time_sec);
    LONGS_EQUAL(3u, result.rotate_90_deg_time_sec);
    LONGS_EQUAL(4u, result.rotate_180_deg_time_sec);
}
