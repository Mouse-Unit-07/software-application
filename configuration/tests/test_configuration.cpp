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

static void check_default_move_forward_common_config(struct move_forward_common_config const& cfg)
{
    LONGS_EQUAL(250, cfg.emergency_stop_threshold);
}

static void
check_default_move_forward_control_no_wall_config(struct move_forward_control_config const& cfg)
{
    LONGS_EQUAL(50, cfg.base_speed);
    LONGS_EQUAL(40, cfg.min_speed);
    LONGS_EQUAL(255, cfg.max_speed);
    LONGS_EQUAL(14, cfg.kp_velocity);
    LONGS_EQUAL(899, cfg.kd_velocity);
    LONGS_EQUAL(66582, cfg.kp_angle);
    LONGS_EQUAL(554, cfg.kd_angle);
    LONGS_EQUAL(0, cfg.kp_ir);
    LONGS_EQUAL(0, cfg.kd_ir);
    LONGS_EQUAL(2165, cfg.pid_scale);
    LONGS_EQUAL(0, cfg.wall_target);
}

static void
check_default_move_forward_control_one_wall_config(struct move_forward_control_config const& cfg)
{
    LONGS_EQUAL(50, cfg.base_speed);
    LONGS_EQUAL(40, cfg.min_speed);
    LONGS_EQUAL(120, cfg.max_speed);
    LONGS_EQUAL(99, cfg.kp_velocity);
    LONGS_EQUAL(407, cfg.kd_velocity);
    LONGS_EQUAL(2709, cfg.kp_angle);
    LONGS_EQUAL(912, cfg.kd_angle);
    LONGS_EQUAL(5092, cfg.kp_ir);
    LONGS_EQUAL(163, cfg.kd_ir);
    LONGS_EQUAL(5629, cfg.pid_scale);
    LONGS_EQUAL(174, cfg.wall_target);
}

static void
check_default_move_forward_control_both_wall_config(struct move_forward_control_config const& cfg)
{
    LONGS_EQUAL(50, cfg.base_speed);
    LONGS_EQUAL(40, cfg.min_speed);
    LONGS_EQUAL(120, cfg.max_speed);
    LONGS_EQUAL(163, cfg.kp_velocity);
    LONGS_EQUAL(219, cfg.kd_velocity);
    LONGS_EQUAL(1542, cfg.kp_angle);
    LONGS_EQUAL(1679, cfg.kd_angle);
    LONGS_EQUAL(9198, cfg.kp_ir);
    LONGS_EQUAL(223, cfg.kd_ir);
    LONGS_EQUAL(7954, cfg.pid_scale);
    LONGS_EQUAL(0, cfg.wall_target);
}

static void check_default_rotate_control_config(struct rotate_control_config const &cfg)
{
    LONGS_EQUAL(65, cfg.base_speed);
    LONGS_EQUAL(60, cfg.min_speed);
    LONGS_EQUAL(70, cfg.max_speed);
    LONGS_EQUAL(1453, cfg.kp_velocity);
    LONGS_EQUAL(446, cfg.kd_velocity);
    LONGS_EQUAL(82643, cfg.kp_angle);
    LONGS_EQUAL(532, cfg.kd_angle);
    LONGS_EQUAL(1823, cfg.pid_scale);
}

static void check_default_front_wall_detection_config(struct front_wall_detection_config const &cfg)
{
    LONGS_EQUAL(161, cfg.reading_threshold);
    LONGS_EQUAL(10, cfg.num_detection_samples);
}

static void check_default_side_wall_detection_config(struct side_wall_detection_config const &cfg)
{
    LONGS_EQUAL(49, cfg.reading_threshold);
    LONGS_EQUAL(77, cfg.slope_threshold);
    LONGS_EQUAL(10, cfg.num_detection_samples);
    DOUBLES_EQUAL(0.1, cfg.reading_start_offset, FLOAT_TOLERANCE);
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

TEST(ConfigurationTests, InitConfigurationResetsMoveForwardCommonConfigToDefaults)
{
    struct move_forward_common_config cfg{};
    cfg.emergency_stop_threshold = 123;

    save_move_forward_common_config_as_test(cfg);

    init_configuration();

    check_default_move_forward_common_config(get_saved_test_move_forward_common_config());
}

TEST(ConfigurationTests, InitConfigurationResetsMoveForwardNoWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_no_wall_config_as_test(cfg);
    init_configuration();
    check_default_move_forward_control_no_wall_config(
        get_saved_test_move_forward_control_no_wall_config());
}

TEST(ConfigurationTests, InitConfigurationResetsMoveForwardOneWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_one_wall_config_as_test(cfg);
    init_configuration();
    check_default_move_forward_control_one_wall_config(
        get_saved_test_move_forward_control_one_wall_config());
}

TEST(ConfigurationTests, InitConfigurationResetsMoveForwardBothWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_both_wall_config_as_test(cfg);
    init_configuration();
    check_default_move_forward_control_both_wall_config(
        get_saved_test_move_forward_control_both_wall_config());
}

TEST(ConfigurationTests, InitConfigurationResetsRotateControlConfigToDefaults)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.pid_scale = 8;

    save_rotate_control_config_as_test(cfg);
    init_configuration();
    check_default_rotate_control_config(get_saved_test_rotate_control_config());
}

TEST(ConfigurationTests, InitConfigurationResetsFrontWallDetectionConfigToDefaults)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.num_detection_samples = 2;

    save_front_wall_detection_config_as_test(cfg);
    init_configuration();
    check_default_front_wall_detection_config(get_saved_test_front_wall_detection_config());
}

TEST(ConfigurationTests, InitConfigurationResetsSideWallDetectionConfigToDefaults)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.slope_threshold = 2;
    cfg.num_detection_samples = 3;
    cfg.reading_start_offset = 4;

    save_side_wall_detection_config_as_test(cfg);
    init_configuration();
    check_default_side_wall_detection_config(get_saved_test_side_wall_detection_config());
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

TEST(ConfigurationTests, DeinitConfigurationResetsMoveForwardCommonConfigToDefaults)
{
    struct move_forward_common_config cfg{};
    cfg.emergency_stop_threshold = 123;

    save_move_forward_common_config_as_test(cfg);

    deinit_configuration();

    check_default_move_forward_common_config(get_saved_test_move_forward_common_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsMoveForwardNoWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_no_wall_config_as_test(cfg);
    deinit_configuration();
    check_default_move_forward_control_no_wall_config(
        get_saved_test_move_forward_control_no_wall_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsMoveForwardOneWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_one_wall_config_as_test(cfg);
    deinit_configuration();
    check_default_move_forward_control_one_wall_config(
        get_saved_test_move_forward_control_one_wall_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsMoveForwardBothWallConfigToDefaults)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_both_wall_config_as_test(cfg);
    deinit_configuration();
    check_default_move_forward_control_both_wall_config(
        get_saved_test_move_forward_control_both_wall_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsRotateControlConfigToDefaults)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.pid_scale = 8;

    save_rotate_control_config_as_test(cfg);
    deinit_configuration();
    check_default_rotate_control_config(get_saved_test_rotate_control_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsFrontWallDetectionConfigToDefaults)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.num_detection_samples = 2;

    save_front_wall_detection_config_as_test(cfg);
    deinit_configuration();
    check_default_front_wall_detection_config(get_saved_test_front_wall_detection_config());
}

TEST(ConfigurationTests, DeinitConfigurationResetsSideWallDetectionConfigToDefaults)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.slope_threshold = 2;
    cfg.num_detection_samples = 3;
    cfg.reading_start_offset = 4;

    save_side_wall_detection_config_as_test(cfg);
    deinit_configuration();
    check_default_side_wall_detection_config(get_saved_test_side_wall_detection_config());
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

TEST(ConfigurationTests, DefaultMoveForwardCommonConfigContainsExpectedValues)
{
    check_default_move_forward_common_config(get_saved_default_move_forward_common_config());
}

TEST(ConfigurationTests, TestMoveForwardCommonConfigInitiallyContainsExpectedValues)
{
    check_default_move_forward_common_config(get_saved_test_move_forward_common_config());
}

TEST(ConfigurationTests, SetTestMoveForwardCommonConfigUpdatesValues)
{
    struct move_forward_common_config cfg{};
    cfg.emergency_stop_threshold = 123;

    save_move_forward_common_config_as_test(cfg);

    struct move_forward_common_config result{get_saved_test_move_forward_common_config()};

    LONGS_EQUAL(123, result.emergency_stop_threshold);
}

TEST(ConfigurationTests, DefaultMoveForwardNoWallConfigContainsExpectedValues)
{
    check_default_move_forward_control_no_wall_config(
        get_saved_default_move_forward_control_no_wall_config());
}

TEST(ConfigurationTests, DefaultMoveForwardOneWallConfigContainsExpectedValues)
{
    check_default_move_forward_control_one_wall_config(
        get_saved_default_move_forward_control_one_wall_config());
}

TEST(ConfigurationTests, DefaultMoveForwardBothWallConfigContainsExpectedValues)
{
    check_default_move_forward_control_both_wall_config(
        get_saved_default_move_forward_control_both_wall_config());
}

TEST(ConfigurationTests, DefaultRotateControlConfigContainsExpectedValues)
{
    check_default_rotate_control_config(get_saved_default_rotate_control_config());
}

TEST(ConfigurationTests, DefaultFrontWallDetectionConfigContainsExpectedValues)
{
    check_default_front_wall_detection_config(get_saved_default_front_wall_detection_config());
}

TEST(ConfigurationTests, DefaultSideWallDetectionConfigContainsExpectedValues)
{
    check_default_side_wall_detection_config(get_saved_default_side_wall_detection_config());
}

TEST(ConfigurationTests, TestMoveForwardNoWallConfigInitiallyContainsExpectedValues)
{
    check_default_move_forward_control_no_wall_config(
        get_saved_test_move_forward_control_no_wall_config());
}

TEST(ConfigurationTests, TestMoveForwardOneWallConfigInitiallyContainsExpectedValues)
{
    check_default_move_forward_control_one_wall_config(
        get_saved_test_move_forward_control_one_wall_config());
}

TEST(ConfigurationTests, TestMoveForwardBothWallConfigInitiallyContainsExpectedValues)
{
    check_default_move_forward_control_both_wall_config(
        get_saved_test_move_forward_control_both_wall_config());
}

TEST(ConfigurationTests, TestRotateControlConfigInitiallyContainsExpectedValues)
{
    check_default_rotate_control_config(get_saved_test_rotate_control_config());
}

TEST(ConfigurationTests, TestFrontWallDetectionConfigInitiallyContainsExpectedValues)
{
    check_default_front_wall_detection_config(get_saved_test_front_wall_detection_config());
}

TEST(ConfigurationTests, TestSideWallDetectionConfigInitiallyContainsExpectedValues)
{
    check_default_side_wall_detection_config(get_saved_test_side_wall_detection_config());
}

TEST(ConfigurationTests, SetTestMoveForwardNoWallConfigUpdatesValues)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_no_wall_config_as_test(cfg);
    struct move_forward_control_config result =
        get_saved_test_move_forward_control_no_wall_config();

    LONGS_EQUAL(1, result.base_speed);
    LONGS_EQUAL(2, result.min_speed);
    LONGS_EQUAL(3, result.max_speed);
    LONGS_EQUAL(4, result.kp_velocity);
    LONGS_EQUAL(5, result.kd_velocity);
    LONGS_EQUAL(6, result.kp_angle);
    LONGS_EQUAL(7, result.kd_angle);
    LONGS_EQUAL(8, result.kp_ir);
    LONGS_EQUAL(9, result.kd_ir);
    LONGS_EQUAL(10, result.pid_scale);
    LONGS_EQUAL(11, result.wall_target);
}

TEST(ConfigurationTests, SetTestMoveForwardOneWallConfigUpdatesValues)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_one_wall_config_as_test(cfg);
    struct move_forward_control_config result =
        get_saved_test_move_forward_control_one_wall_config();

    LONGS_EQUAL(1, result.base_speed);
    LONGS_EQUAL(2, result.min_speed);
    LONGS_EQUAL(3, result.max_speed);
    LONGS_EQUAL(4, result.kp_velocity);
    LONGS_EQUAL(5, result.kd_velocity);
    LONGS_EQUAL(6, result.kp_angle);
    LONGS_EQUAL(7, result.kd_angle);
    LONGS_EQUAL(8, result.kp_ir);
    LONGS_EQUAL(9, result.kd_ir);
    LONGS_EQUAL(10, result.pid_scale);
    LONGS_EQUAL(11, result.wall_target);
}

TEST(ConfigurationTests, SetTestMoveForwardBothWallConfigUpdatesValues)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.kp_ir = 8;
    cfg.kd_ir = 9;
    cfg.pid_scale = 10;
    cfg.wall_target = 11;

    save_move_forward_control_both_wall_config_as_test(cfg);
    struct move_forward_control_config result =
        get_saved_test_move_forward_control_both_wall_config();

    LONGS_EQUAL(1, result.base_speed);
    LONGS_EQUAL(2, result.min_speed);
    LONGS_EQUAL(3, result.max_speed);
    LONGS_EQUAL(4, result.kp_velocity);
    LONGS_EQUAL(5, result.kd_velocity);
    LONGS_EQUAL(6, result.kp_angle);
    LONGS_EQUAL(7, result.kd_angle);
    LONGS_EQUAL(8, result.kp_ir);
    LONGS_EQUAL(9, result.kd_ir);
    LONGS_EQUAL(10, result.pid_scale);
    LONGS_EQUAL(11, result.wall_target);
}

TEST(ConfigurationTests, SetTestRotateControlConfigUpdatesValues)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 1;
    cfg.min_speed = 2;
    cfg.max_speed = 3;
    cfg.kp_velocity = 4;
    cfg.kd_velocity = 5;
    cfg.kp_angle = 6;
    cfg.kd_angle = 7;
    cfg.pid_scale = 8;

    save_rotate_control_config_as_test(cfg);
    struct rotate_control_config result = get_saved_test_rotate_control_config();

    LONGS_EQUAL(1, result.base_speed);
    LONGS_EQUAL(2, result.min_speed);
    LONGS_EQUAL(3, result.max_speed);
    LONGS_EQUAL(4, result.kp_velocity);
    LONGS_EQUAL(5, result.kd_velocity);
    LONGS_EQUAL(6, result.kp_angle);
    LONGS_EQUAL(7, result.kd_angle);
    LONGS_EQUAL(8, result.pid_scale);
}

TEST(ConfigurationTests, SetTestFrontWallDetectionConfigUpdatesValues)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.num_detection_samples = 2;

    save_front_wall_detection_config_as_test(cfg);
    struct front_wall_detection_config result = get_saved_test_front_wall_detection_config();

    LONGS_EQUAL(1, result.reading_threshold);
    LONGS_EQUAL(2, result.num_detection_samples);
}

TEST(ConfigurationTests, SetTestSideWallDetectionConfigUpdatesValues)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 1;
    cfg.slope_threshold = 2;
    cfg.num_detection_samples = 3;
    cfg.reading_start_offset = 4.0;

    save_side_wall_detection_config_as_test(cfg);
    struct side_wall_detection_config result = get_saved_test_side_wall_detection_config();

    LONGS_EQUAL(1, result.reading_threshold);
    LONGS_EQUAL(2, result.slope_threshold);
    LONGS_EQUAL(3, result.num_detection_samples);
    DOUBLES_EQUAL(4.0, result.reading_start_offset, FLOAT_TOLERANCE);
}
