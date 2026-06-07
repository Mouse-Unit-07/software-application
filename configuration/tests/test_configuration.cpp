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
#include "navigation.h"
#include "configuration.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
constexpr double FLOAT_TOLERANCE{1e-6};

static void check_default_maze_solver_config(struct maze_solver_config const &cfg)
{
    LONGS_EQUAL(16u, cfg.maze_size);
    LONGS_EQUAL(600u, cfg.total_timeout_sec);
    LONGS_EQUAL(1u, cfg.move_forward_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_90_deg_time_sec);
    LONGS_EQUAL(1u, cfg.rotate_180_deg_time_sec);
}

static void check_default_mouse_physical_params(struct mouse_physical_params const &cfg)
{
    DOUBLES_EQUAL(32.0, cfg.wheel_diameter_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(87.56, cfg.wheel_base_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(4900.0, cfg.max_motor_rpm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(60.8077, cfg.encoder_events_per_revolution, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(13.0, cfg.motor_pinion_gear_teeth, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(44.0, cfg.wheel_gear_teeth, FLOAT_TOLERANCE);
}

static void check_default_maze_physical_params(struct maze_physical_params const &cfg)
{
    DOUBLES_EQUAL(12.07, cfg.post_size_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(166.37, cfg.wall_size_mm, FLOAT_TOLERANCE);
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

    save_maze_solver_config_as_test(cfg);

    init_configuration();

    struct maze_solver_config result{get_saved_test_maze_solver_config()};

    check_default_maze_solver_config(result);
}

TEST(ConfigurationTests, InitConfigurationResetsMousePhysicalParamsToDefaults)
{
    struct mouse_physical_params cfg{};
    cfg.wheel_diameter_mm = 1.0;
    cfg.wheel_base_mm = 2.0;
    cfg.max_motor_rpm = 3.0;
    cfg.encoder_events_per_revolution = 4.0;
    cfg.motor_pinion_gear_teeth = 5.0;
    cfg.wheel_gear_teeth = 6.0;

    save_mouse_physical_params_as_test(cfg);

    init_configuration();

    check_default_mouse_physical_params(get_saved_test_mouse_physical_params());
}

TEST(ConfigurationTests, InitConfigurationResetsMazePhysicalParamsToDefaults)
{
    struct maze_physical_params cfg{};
    cfg.post_size_mm = 1.0;
    cfg.wall_size_mm = 2.0;

    save_maze_physical_params_as_test(cfg);

    init_configuration();

    check_default_maze_physical_params(get_saved_test_maze_physical_params());
}

TEST(ConfigurationTests, DeinitConfigurationResetsTestConfigToDefaults)
{
    struct maze_solver_config cfg{0};
    cfg.maze_size = 8u;
    cfg.total_timeout_sec = 123u;
    cfg.move_forward_time_sec = 2u;
    cfg.rotate_90_deg_time_sec = 3u;
    cfg.rotate_180_deg_time_sec = 4u;

    save_maze_solver_config_as_test(cfg);

    deinit_configuration();

    struct maze_solver_config result{get_saved_test_maze_solver_config()};

    check_default_maze_solver_config(result);
}

TEST(ConfigurationTests, DeinitConfigurationResetsMousePhysicalParamsToDefaults)
{
    struct mouse_physical_params cfg{};
    cfg.wheel_diameter_mm = 1.0;
    cfg.wheel_base_mm = 2.0;
    cfg.max_motor_rpm = 3.0;
    cfg.encoder_events_per_revolution = 4.0;
    cfg.motor_pinion_gear_teeth = 5.0;
    cfg.wheel_gear_teeth = 6.0;

    save_mouse_physical_params_as_test(cfg);

    deinit_configuration();

    check_default_mouse_physical_params(get_saved_test_mouse_physical_params());
}

TEST(ConfigurationTests, DeinitConfigurationResetsMazePhysicalParamsToDefaults)
{
    struct maze_physical_params cfg{};
    cfg.post_size_mm = 1.0;
    cfg.wall_size_mm = 2.0;

    save_maze_physical_params_as_test(cfg);

    deinit_configuration();

    check_default_maze_physical_params(get_saved_test_maze_physical_params());
}

TEST(ConfigurationTests, DefaultMazeSolverConfigContainsExpectedValues)
{
    struct maze_solver_config cfg{get_saved_default_maze_solver_config()};

    check_default_maze_solver_config(cfg);
}

TEST(ConfigurationTests, TestMazeSolverConfigInitiallyContainsExpectedValues)
{
    struct maze_solver_config cfg{get_saved_test_maze_solver_config()};

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

    save_maze_solver_config_as_test(cfg);

    struct maze_solver_config result{get_saved_test_maze_solver_config()};

    LONGS_EQUAL(8u, result.maze_size);
    LONGS_EQUAL(123u, result.total_timeout_sec);
    LONGS_EQUAL(2u, result.move_forward_time_sec);
    LONGS_EQUAL(3u, result.rotate_90_deg_time_sec);
    LONGS_EQUAL(4u, result.rotate_180_deg_time_sec);
}

TEST(ConfigurationTests, DefaultMousePhysicalParamsContainExpectedValues)
{
    check_default_mouse_physical_params(get_saved_default_mouse_physical_params());
}

TEST(ConfigurationTests, TestMousePhysicalParamsInitiallyContainExpectedValues)
{
    check_default_mouse_physical_params(get_saved_test_mouse_physical_params());
}

TEST(ConfigurationTests, SetTestMousePhysicalParamsUpdatesValues)
{
    struct mouse_physical_params cfg{};
    cfg.wheel_diameter_mm = 1.0;
    cfg.wheel_base_mm = 2.0;
    cfg.max_motor_rpm = 3.0;
    cfg.encoder_events_per_revolution = 4.0;
    cfg.motor_pinion_gear_teeth = 5.0;
    cfg.wheel_gear_teeth = 6.0;

    save_mouse_physical_params_as_test(cfg);

    struct mouse_physical_params result = get_saved_test_mouse_physical_params();

    DOUBLES_EQUAL(1.0, result.wheel_diameter_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(2.0, result.wheel_base_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(3.0, result.max_motor_rpm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(4.0, result.encoder_events_per_revolution, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(5.0, result.motor_pinion_gear_teeth, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(6.0, result.wheel_gear_teeth, FLOAT_TOLERANCE);
}

TEST(ConfigurationTests, DefaultMazePhysicalParamsContainExpectedValues)
{
    check_default_maze_physical_params(get_saved_default_maze_physical_params());
}

TEST(ConfigurationTests, TestMazePhysicalParamsInitiallyContainExpectedValues)
{
    check_default_maze_physical_params(get_saved_test_maze_physical_params());
}

TEST(ConfigurationTests, SetTestMazePhysicalParamsUpdatesValues)
{
    struct maze_physical_params cfg{};
    cfg.post_size_mm = 1.0;
    cfg.wall_size_mm = 2.0;

    save_maze_physical_params_as_test(cfg);

    struct maze_physical_params result = get_saved_test_maze_physical_params();

    DOUBLES_EQUAL(1.0, result.post_size_mm, FLOAT_TOLERANCE);
    DOUBLES_EQUAL(2.0, result.wall_size_mm, FLOAT_TOLERANCE);
}
