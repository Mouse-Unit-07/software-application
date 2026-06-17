/*================================ FILE INFO =================================*/
/* Filename           : test_set.cpp                                          */
/*                                                                            */
/* Test implementation for set.c                                              */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdint.h>
#include "navigation.h"
#include "maze_solver_common.h"
#include "command.h"
#include "set.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
constexpr uint32_t FAKE_ROOT_COMMANDS_COUNT{1u};

struct command_node fake_root_commands[FAKE_ROOT_COMMANDS_COUNT]{{0}};

void init_test_fakes(void)
{
    fake_root_commands[0].name = "set";
    fake_root_commands[0].help =
        "Select configuration values to use w/ or w/o new values for test configs";
    fake_root_commands[0].validate = validate_set;
    fake_root_commands[0].execute = execute_set;
    fake_root_commands[0].children = get_set_commands();
    fake_root_commands[0].child_count = get_set_commands_count();
}

void deinit_test_fakes(void)
{
    fake_root_commands[0] = {};
}

struct command make_set_command(char const *subcommand, uint32_t token_count)
{
    struct command cmd{{0}};

    cmd.token_count = token_count;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = subcommand;

    return cmd;
}

void check_command_lookup(char const *command_name, uint32_t token_count)
{
    struct command cmd{make_set_command(command_name, token_count)};

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL(command_name, node->name);
}

void check_validation_success(char const *command_name, uint32_t token_count,
                              enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_set_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate(&cmd));
}

void check_validation_too_many_params(char const *command_name, uint32_t token_count,
                                      enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_set_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate(&cmd));
}

void check_validation_too_few_params(char const *command_name, uint32_t token_count,
                                     enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_set_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate(&cmd));
}

struct validation_test_case {
    const char *command_name;
    enum validation_result (*validate)(struct command *);
    uint32_t min_token_count;
    uint32_t max_token_count;
};

static const validation_test_case validation_test_cases[] = {
    {"solver-default", validate_set_solver_default, 2, 2},
    {"solver-test", validate_set_solver_test, 2, 7},
    {"mouse-physical-default", validate_set_mouse_physical_default, 2, 2},
    {"mouse-physical-test", validate_set_mouse_physical_test, 2, 8},
    {"maze-physical-default", validate_set_maze_physical_default, 2, 2},
    {"maze-physical-test", validate_set_maze_physical_test, 2, 4},
    {"move-forward-common-default", validate_set_move_forward_common_default, 2, 2},
    {"move-forward-common-test", validate_set_move_forward_common_test, 2, 3},
    {"move-forward-no-wall-default", validate_set_move_forward_no_wall_default, 2, 2},
    {"move-forward-no-wall-test", validate_set_move_forward_no_wall_test, 2, 13},
    {"move-forward-one-wall-default", validate_set_move_forward_one_wall_default, 2, 2},
    {"move-forward-one-wall-test", validate_set_move_forward_one_wall_test, 2, 13},
    {"move-forward-both-wall-default", validate_set_move_forward_both_wall_default, 2, 2},
    {"move-forward-both-wall-test", validate_set_move_forward_both_wall_test, 2, 13},
    {"rotate-default", validate_set_rotate_default, 2, 2},
    {"rotate-test", validate_set_rotate_test, 2, 10},
    {"front-wall-default", validate_set_front_wall_default, 2, 2},
    {"front-wall-test", validate_set_front_wall_test, 2, 4},
    {"side-wall-default", validate_set_side_wall_default, 2, 2},
    {"side-wall-test", validate_set_side_wall_test, 2, 6}};

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

struct maze_solver_config get_saved_default_maze_solver_config(void)
{
    struct maze_solver_config cfg{};
    cfg.maze_size = 16u;
    return cfg;
}

struct maze_solver_config get_saved_test_maze_solver_config(void)
{
    struct maze_solver_config cfg{};
    cfg.maze_size = 8u;
    return cfg;
}

void set_maze_solver_config(struct maze_solver_config cfg)
{
    mock().actualCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", cfg.maze_size);
}

void save_maze_solver_config_as_test(struct maze_solver_config cfg)
{
    mock().actualCall("save_maze_solver_config_as_test")
        .withUnsignedIntParameter("maze_size", cfg.maze_size)
        .withUnsignedIntParameter("total_timeout_sec", cfg.total_timeout_sec)
        .withUnsignedIntParameter("move_forward_time_sec", cfg.move_forward_time_sec)
        .withUnsignedIntParameter("rotate_90_deg_time_sec", cfg.rotate_90_deg_time_sec)
        .withUnsignedIntParameter("rotate_180_deg_time_sec", cfg.rotate_180_deg_time_sec);
}

struct mouse_physical_params get_saved_default_mouse_physical_params(void)
{
    struct mouse_physical_params p{};
    p.wheel_diameter_mm = 32.0f;
    return p;
}

struct mouse_physical_params get_saved_test_mouse_physical_params(void)
{
    struct mouse_physical_params p{};
    p.wheel_diameter_mm = 40.0f;
    return p;
}

void save_mouse_physical_params_as_test(struct mouse_physical_params p)
{
    mock().actualCall("save_mouse_physical_params_as_test")
        .withDoubleParameter("wheel_diameter_mm", p.wheel_diameter_mm);
}

void calculate_mouse_params(struct mouse_physical_params p)
{
    mock().actualCall("calculate_mouse_params")
        .withDoubleParameter("wheel_diameter_mm", p.wheel_diameter_mm);
}

struct maze_physical_params get_saved_default_maze_physical_params(void)
{
    struct maze_physical_params p{};
    p.post_size_mm = 12.0f;
    return p;
}

struct maze_physical_params get_saved_test_maze_physical_params(void)
{
    struct maze_physical_params p{};
    p.post_size_mm = 20.0f;
    return p;
}

void save_maze_physical_params_as_test(struct maze_physical_params p)
{
    mock().actualCall("save_maze_physical_params_as_test")
        .withDoubleParameter("post_size_mm", p.post_size_mm)
        .withDoubleParameter("wall_size_mm", p.wall_size_mm);
}

void calculate_maze_params(struct maze_physical_params p)
{
    mock().actualCall("calculate_maze_params")
        .withDoubleParameter("post_size_mm", p.post_size_mm)
        .withDoubleParameter("wall_size_mm", p.wall_size_mm);
}

void calculate_navigation_params(void)
{
    mock().actualCall("calculate_navigation_params");
}

struct move_forward_common_config get_saved_default_move_forward_common_config(void)
{
    struct move_forward_common_config cfg{};
    cfg.emergency_stop_threshold = 500u;
    return cfg;
}

struct move_forward_common_config get_saved_test_move_forward_common_config(void)
{
    struct move_forward_common_config cfg{};
    cfg.emergency_stop_threshold = 800u;
    return cfg;
}

void save_move_forward_common_config_as_test(struct move_forward_common_config cfg)
{
    mock().actualCall("save_move_forward_common_config_as_test")
        .withUnsignedIntParameter("emergency_stop_threshold", cfg.emergency_stop_threshold);
}

void set_move_forward_common_config(struct move_forward_common_config cfg)
{
    mock().actualCall("set_move_forward_common_config")
        .withUnsignedIntParameter("emergency_stop_threshold", cfg.emergency_stop_threshold);
}


struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 100u;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_no_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 200u;
    return cfg;
}

void save_move_forward_control_no_wall_config_as_test(struct move_forward_control_config cfg)
{
    mock().actualCall("save_move_forward_control_no_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

void set_no_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    mock().actualCall("set_no_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 101u;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_one_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 201u;
    return cfg;
}

void save_move_forward_control_one_wall_config_as_test(struct move_forward_control_config cfg)
{
    mock().actualCall("save_move_forward_control_one_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

void set_one_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    mock().actualCall("set_one_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 102u;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_both_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.wall_target = 202u;
    return cfg;
}

void save_move_forward_control_both_wall_config_as_test(struct move_forward_control_config cfg)
{
    mock().actualCall("save_move_forward_control_both_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

void set_both_wall_move_forward_control_config(struct move_forward_control_config cfg)
{
    mock().actualCall("set_both_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("kp_ir", cfg.kp_ir)
        .withLongIntParameter("kd_ir", cfg.kd_ir)
        .withLongIntParameter("pid_scale", cfg.pid_scale)
        .withUnsignedIntParameter("wall_target", cfg.wall_target);
}

struct rotate_control_config get_saved_default_rotate_control_config(void)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 10u;
    return cfg;
}

struct rotate_control_config get_saved_test_rotate_control_config(void)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 20u;
    return cfg;
}

void save_rotate_control_config_as_test(struct rotate_control_config cfg)
{
    mock().actualCall("save_rotate_control_config_as_test")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("pid_scale", cfg.pid_scale);
}

void set_rotate_control_config(struct rotate_control_config cfg)
{
    mock().actualCall("set_rotate_control_config")
        .withUnsignedIntParameter("base_speed", cfg.base_speed)
        .withUnsignedIntParameter("min_speed", cfg.min_speed)
        .withUnsignedIntParameter("max_speed", cfg.max_speed)
        .withLongIntParameter("kp_velocity", cfg.kp_velocity)
        .withLongIntParameter("kd_velocity", cfg.kd_velocity)
        .withLongIntParameter("kp_angle", cfg.kp_angle)
        .withLongIntParameter("kd_angle", cfg.kd_angle)
        .withLongIntParameter("pid_scale", cfg.pid_scale);
}

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 100u;
    return cfg;
}

struct front_wall_detection_config get_saved_test_front_wall_detection_config(void)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 200u;
    return cfg;
}

void save_front_wall_detection_config_as_test(struct front_wall_detection_config cfg)
{
    mock().actualCall("save_front_wall_detection_config_as_test")
        .withUnsignedIntParameter("reading_threshold", cfg.reading_threshold)
        .withUnsignedIntParameter("num_detection_samples", cfg.num_detection_samples);
}

void set_front_wall_detection_config(struct front_wall_detection_config cfg)
{
    mock().actualCall("set_front_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", cfg.reading_threshold)
        .withUnsignedIntParameter("num_detection_samples", cfg.num_detection_samples);
}

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 100u;
    return cfg;
}

struct side_wall_detection_config get_saved_test_side_wall_detection_config(void)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 200u;
    return cfg;
}

void save_side_wall_detection_config_as_test(struct side_wall_detection_config cfg)
{
    mock().actualCall("save_side_wall_detection_config_as_test")
        .withUnsignedIntParameter("reading_threshold", cfg.reading_threshold)
        .withUnsignedIntParameter("slope_threshold", cfg.slope_threshold)
        .withUnsignedIntParameter("num_detection_samples", cfg.num_detection_samples)
        .withDoubleParameter("reading_start_offset", cfg.reading_start_offset);
}

void set_side_wall_detection_config(struct side_wall_detection_config cfg)
{
    mock().actualCall("set_side_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", cfg.reading_threshold)
        .withUnsignedIntParameter("slope_threshold", cfg.slope_threshold)
        .withUnsignedIntParameter("num_detection_samples", cfg.num_detection_samples)
        .withDoubleParameter("reading_start_offset", cfg.reading_start_offset);
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(SetTests)
{
    void setup() override
    {
        init_test_fakes();
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
        deinit_test_fakes();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(SetTests, GetSetNodeReturnsValidNode)
{
    const struct command_node *node = get_set_node();

    CHECK(node != nullptr);
    STRCMP_EQUAL("set", node->name);
    POINTERS_EQUAL(validate_set, node->validate);
    POINTERS_EQUAL(execute_set, node->execute);
    POINTERS_EQUAL(get_set_commands(), node->children);
    LONGS_EQUAL(get_set_commands_count(), node->child_count);
}

TEST(SetTests, SetCommandsAreConfiguredCorrectly)
{
    const struct command_node *commands = get_set_commands();

    LONGS_EQUAL(sizeof(validation_test_cases) / sizeof(validation_test_cases[0]),
                get_set_commands_count());

    for (uint32_t i = 0; i < get_set_commands_count(); i++) {
        STRCMP_EQUAL(validation_test_cases[i].command_name, commands[i].name);
        POINTERS_EQUAL(validation_test_cases[i].validate, commands[i].validate);
        CHECK(commands[i].execute != nullptr);
    }
}

TEST(SetTests, AllCommandNodesExist)
{
    for (uint32_t i = 0; i < sizeof(validation_test_cases) / sizeof(validation_test_cases[0]);
         i++) {
        check_command_lookup(validation_test_cases[i].command_name,
                             validation_test_cases[i].min_token_count);
    }
}

TEST(SetTests, ValidationTokenCountsAreCorrect)
{
    for (uint32_t i = 0; i < sizeof(validation_test_cases) / sizeof(validation_test_cases[0]);
         i++) {
        const validation_test_case& test_case = validation_test_cases[i];

        for (uint32_t token_count = test_case.min_token_count;
             token_count <= test_case.max_token_count + 1; token_count++) {
            struct command cmd = make_set_command(test_case.command_name, token_count);
            for (uint32_t j = 2; j < token_count; j++) {
                cmd.tokens[j] = "0";
            }

            enum validation_result expected;

            if ((token_count == test_case.min_token_count)
                || (token_count == test_case.max_token_count)) {
                expected = COMMAND_VALIDATION_SUCCESS;
            } else if (token_count < test_case.max_token_count) {
                expected = COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
            } else {
                expected = COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
            }

            enum validation_result actual = test_case.validate(&cmd);
            SimpleString msg =
                StringFromFormat("command=%s token_count=%u expected=%d actual=%d",
                                 test_case.command_name, token_count, expected, actual);

            LONGS_EQUAL_TEXT(expected, actual, msg.asCharString());
        }
    }
}

/*----------------------------------------------------------------------------*/
/* set */
TEST(SetTests, FindCommandNodeReturnsSetNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "set";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("set", node->name);
}

TEST(SetTests, ValidateSetReturnsSuccess)
{
    struct command cmd{make_set_command("", 1)};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set(&cmd));
}

TEST(SetTests, ValidateSetReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set(&cmd));
}

TEST(SetTests, ExecuteSetRuns)
{
    struct command cmd{{0}};

    execute_set(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
TEST(SetTests, ExecuteSetSolverDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 16u);

    execute_set_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set solver-test */
TEST(SetTests, ValidateSetSolverTestReturnsSuccessForEditConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 7;
    cmd.tokens[2] = "16";
    cmd.tokens[3] = "60";
    cmd.tokens[4] = "1";
    cmd.tokens[5] = "1";
    cmd.tokens[6] = "2";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_test(&cmd));
}

TEST(SetTests, ValidateSetSolverTestReturnsBadParameter)
{
    struct command cmd{{0}};
    cmd.token_count = 7;
    cmd.tokens[2] = "17";
    cmd.tokens[3] = "60";
    cmd.tokens[4] = "1";
    cmd.tokens[5] = "1";
    cmd.tokens[6] = "2";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_solver_test(&cmd));
    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

TEST(SetTests, ValidateSetSolverTestAcceptsMaximumMazeSize)
{
    struct command cmd{{0}};
    cmd.token_count = 7;
    cmd.tokens[2] = "16";
    cmd.tokens[3] = "60";
    cmd.tokens[4] = "1";
    cmd.tokens[5] = "1";
    cmd.tokens[6] = "2";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_test(&cmd));
}

TEST(SetTests, ExecuteSetSolverTestUsesStoredTestConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 8u);

    execute_set_solver_test(&cmd);
}

TEST(SetTests, ExecuteSetSolverTestUpdatesTestConfig)
{
    struct command cmd{{0}};

    cmd.token_count = 7;

    cmd.tokens[2] = "16";
    cmd.tokens[3] = "100";
    cmd.tokens[4] = "10";
    cmd.tokens[5] = "20";
    cmd.tokens[6] = "40";

    mock().expectOneCall("save_maze_solver_config_as_test")
        .withUnsignedIntParameter("maze_size", 16u)
        .withUnsignedIntParameter("total_timeout_sec", 100u)
        .withUnsignedIntParameter("move_forward_time_sec", 10u)
        .withUnsignedIntParameter("rotate_90_deg_time_sec", 20u)
        .withUnsignedIntParameter("rotate_180_deg_time_sec", 40u);

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 16u);

    execute_set_solver_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set mouse-physical-default */
TEST(SetTests, ExecuteSetMousePhysicalDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("calculate_mouse_params")
        .withDoubleParameter("wheel_diameter_mm", 32.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_mouse_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set mouse-physical-test */
TEST(SetTests, ExecuteSetMousePhysicalTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("calculate_mouse_params")
        .withDoubleParameter("wheel_diameter_mm", 40.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_mouse_physical_test(&cmd);
}

TEST(SetTests, ExecuteSetMousePhysicalTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 8;
    cmd.tokens[2] = "32";
    cmd.tokens[3] = "72";
    cmd.tokens[4] = "30000";
    cmd.tokens[5] = "12";
    cmd.tokens[6] = "9";
    cmd.tokens[7] = "27";

    mock().expectOneCall("save_mouse_physical_params_as_test")
        .withDoubleParameter("wheel_diameter_mm", 32.0);;

    mock().expectOneCall("calculate_mouse_params")
        .withDoubleParameter("wheel_diameter_mm", 32.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_mouse_physical_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set maze-physical-default */
TEST(SetTests, ExecuteSetMazePhysicalDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("calculate_maze_params")
        .withDoubleParameter("post_size_mm", 12.0)
        .withDoubleParameter("wall_size_mm", 0.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_maze_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set maze-physical-test */
TEST(SetTests, ExecuteSetMazePhysicalTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("calculate_maze_params")
        .withDoubleParameter("post_size_mm", 20.0)
        .withDoubleParameter("wall_size_mm", 0.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_maze_physical_test(&cmd);
}

TEST(SetTests, ExecuteSetMazePhysicalTestUpdatesConfig)
{
    struct command cmd{{0}};

    cmd.token_count = 4;
    cmd.tokens[2] = "12";
    cmd.tokens[3] = "6";

    mock().expectOneCall("save_maze_physical_params_as_test")
        .withDoubleParameter("post_size_mm", 12.0)
        .withDoubleParameter("wall_size_mm", 6.0);

    mock().expectOneCall("calculate_maze_params")
        .withDoubleParameter("post_size_mm", 12.0)
        .withDoubleParameter("wall_size_mm", 6.0);

    mock().expectOneCall("calculate_navigation_params");

    execute_set_maze_physical_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-common-default */
TEST(SetTests, ExecuteSetMoveForwardCommonDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_move_forward_common_config")
        .withUnsignedIntParameter("emergency_stop_threshold", 500u);

    execute_set_move_forward_common_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-common-test */
TEST(SetTests, ExecuteSetMoveForwardCommonTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_move_forward_common_config")
        .withUnsignedIntParameter("emergency_stop_threshold", 800u);

    execute_set_move_forward_common_test(&cmd);
}

TEST(SetTests, ExecuteSetMoveForwardCommonTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    cmd.tokens[2] = "10";

    mock().expectOneCall("save_move_forward_common_config_as_test")
        .withUnsignedIntParameter("emergency_stop_threshold", 10u);

    mock().expectOneCall("set_move_forward_common_config")
        .withUnsignedIntParameter("emergency_stop_threshold", 10u);

    execute_set_move_forward_common_test(&cmd);
}

TEST(SetTests, ValidateSetMoveForwardCommonTestAccepts1022)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    cmd.tokens[2] = "1022";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_move_forward_common_test(&cmd));
}

TEST(SetTests, ValidateSetMoveForwardCommonTestRejects1023)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    cmd.tokens[2] = "1023";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_move_forward_common_test(&cmd));
    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-no-wall-default */
TEST(SetTests, ExecuteSetMoveForwardNoWallDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_no_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 100u);

    execute_set_move_forward_no_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-no-wall-test */
TEST(SetTests, ExecuteSetMoveForwardNoWallTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_no_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 200u);

    execute_set_move_forward_no_wall_test(&cmd);
}

TEST(SetTests, ExecuteSetMoveForwardNoWallTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "12";
    cmd.tokens[5] = "13";
    cmd.tokens[6] = "14";
    cmd.tokens[7] = "15";
    cmd.tokens[8] = "16";
    cmd.tokens[9] = "17";
    cmd.tokens[10] = "18";
    cmd.tokens[11] = "19";
    cmd.tokens[12] = "20";

    mock().expectOneCall("save_move_forward_control_no_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    mock().expectOneCall("set_no_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    execute_set_move_forward_no_wall_test(&cmd);
}

TEST(SetTests, ValidateSetMoveForwardNoWallTestRejectsWallTargetGreaterThan1023)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    for (uint32_t i = 2; i < 12; i++) {
        cmd.tokens[i] = "0";
    }

    cmd.tokens[12] = "1024";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_move_forward_no_wall_test(&cmd));

    LONGS_EQUAL(12u, cmd.bad_parameter_index);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-one-wall-default */
TEST(SetTests, ExecuteSetMoveForwardOneWallDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_one_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 101u);

    execute_set_move_forward_one_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-one-wall-test */
TEST(SetTests, ExecuteSetMoveForwardOneWallTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_one_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 201u);

    execute_set_move_forward_one_wall_test(&cmd);
}

TEST(SetTests, ExecuteSetMoveForwardOneWallTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "12";
    cmd.tokens[5] = "13";
    cmd.tokens[6] = "14";
    cmd.tokens[7] = "15";
    cmd.tokens[8] = "16";
    cmd.tokens[9] = "17";
    cmd.tokens[10] = "18";
    cmd.tokens[11] = "19";
    cmd.tokens[12] = "20";

    mock().expectOneCall("save_move_forward_control_one_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    mock().expectOneCall("set_one_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    execute_set_move_forward_one_wall_test(&cmd);
}

TEST(SetTests, ValidateSetMoveForwardOneWallTestRejectsWallTargetGreaterThan1023)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    for (uint32_t i = 2; i < 12; i++) {
        cmd.tokens[i] = "0";
    }

    cmd.tokens[12] = "1024";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_move_forward_one_wall_test(&cmd));

    LONGS_EQUAL(12u, cmd.bad_parameter_index);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-both-wall-default */
TEST(SetTests, ExecuteSetMoveForwardBothWallDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_both_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 102u);

    execute_set_move_forward_both_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set move-forward-both-wall-test */
TEST(SetTests, ExecuteSetMoveForwardBothWallTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_both_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 0)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("kp_ir", 0)
        .withLongIntParameter("kd_ir", 0)
        .withLongIntParameter("pid_scale", 0)
        .withUnsignedIntParameter("wall_target", 202u);

    execute_set_move_forward_both_wall_test(&cmd);
}

TEST(SetTests, ExecuteSetMoveForwardBothWallTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "12";
    cmd.tokens[5] = "13";
    cmd.tokens[6] = "14";
    cmd.tokens[7] = "15";
    cmd.tokens[8] = "16";
    cmd.tokens[9] = "17";
    cmd.tokens[10] = "18";
    cmd.tokens[11] = "19";
    cmd.tokens[12] = "20";

    mock().expectOneCall("save_move_forward_control_both_wall_config_as_test")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    mock().expectOneCall("set_both_wall_move_forward_control_config")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("kp_ir", 17)
        .withLongIntParameter("kd_ir", 18)
        .withLongIntParameter("pid_scale", 19)
        .withUnsignedIntParameter("wall_target", 20);

    execute_set_move_forward_both_wall_test(&cmd);
}

TEST(SetTests, ValidateSetMoveForwardBothWallTestRejectsWallTargetGreaterThan1023)
{
    struct command cmd{{0}};
    cmd.token_count = 13;

    for (uint32_t i = 2; i < 12; i++) {
        cmd.tokens[i] = "0";
    }

    cmd.tokens[12] = "1024";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_move_forward_both_wall_test(&cmd));

    LONGS_EQUAL(12u, cmd.bad_parameter_index);
}

/*----------------------------------------------------------------------------*/
/* set rotate-default */
TEST(SetTests, ExecuteSetRotateDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_rotate_control_config")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("pid_scale", 0);

    execute_set_rotate_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set rotate-test */
TEST(SetTests, ExecuteSetRotateTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_rotate_control_config")
        .withUnsignedIntParameter("base_speed", 20u)
        .withUnsignedIntParameter("min_speed", 0)
        .withUnsignedIntParameter("max_speed", 0)
        .withLongIntParameter("kp_velocity", 0)
        .withLongIntParameter("kd_velocity", 0)
        .withLongIntParameter("kp_angle", 0)
        .withLongIntParameter("kd_angle", 0)
        .withLongIntParameter("pid_scale", 0);

    execute_set_rotate_test(&cmd);
}

TEST(SetTests, ExecuteSetRotateTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 10;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "12";
    cmd.tokens[5] = "13";
    cmd.tokens[6] = "14";
    cmd.tokens[7] = "15";
    cmd.tokens[8] = "16";
    cmd.tokens[9] = "17";

    mock().expectOneCall("save_rotate_control_config_as_test")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("pid_scale", 17);

    mock().expectOneCall("set_rotate_control_config")
        .withUnsignedIntParameter("base_speed", 10u)
        .withUnsignedIntParameter("min_speed", 11u)
        .withUnsignedIntParameter("max_speed", 12u)
        .withLongIntParameter("kp_velocity", 13)
        .withLongIntParameter("kd_velocity", 14)
        .withLongIntParameter("kp_angle", 15)
        .withLongIntParameter("kd_angle", 16)
        .withLongIntParameter("pid_scale", 17);

    execute_set_rotate_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set front-wall-default */
TEST(SetTests, ExecuteSetFrontWallDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_front_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 100u)
        .withUnsignedIntParameter("num_detection_samples", 0);

    execute_set_front_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set front-wall-test */
TEST(SetTests, ExecuteSetFrontWallTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_front_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 200u)
        .withUnsignedIntParameter("num_detection_samples", 0);

    execute_set_front_wall_test(&cmd);
}

TEST(SetTests, ExecuteSetFrontWallTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";

    mock().expectOneCall("save_front_wall_detection_config_as_test")
        .withUnsignedIntParameter("reading_threshold", 10u)
        .withUnsignedIntParameter("num_detection_samples", 11u);

    mock().expectOneCall("set_front_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 10u)
        .withUnsignedIntParameter("num_detection_samples", 11u);

    execute_set_front_wall_test(&cmd);
}

TEST(SetTests, ValidateSetFrontWallTestAccepts1022)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    cmd.tokens[2] = "1022";
    cmd.tokens[3] = "5";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_front_wall_test(&cmd));
}

TEST(SetTests, ValidateSetFrontWallTestRejects1023)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    cmd.tokens[2] = "1023";
    cmd.tokens[3] = "5";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_front_wall_test(&cmd));
    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

/*----------------------------------------------------------------------------*/
/* set side-wall-default */
TEST(SetTests, ExecuteSetSideWallDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_side_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 100u)
        .withUnsignedIntParameter("slope_threshold", 0)
        .withUnsignedIntParameter("num_detection_samples", 0)
        .withDoubleParameter("reading_start_offset", 0);

    execute_set_side_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set side-wall-test */
TEST(SetTests, ExecuteSetSideWallTestUsesStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_side_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 200u)
        .withUnsignedIntParameter("slope_threshold", 0)
        .withUnsignedIntParameter("num_detection_samples", 0)
        .withDoubleParameter("reading_start_offset", 0.0);

    execute_set_side_wall_test(&cmd);
}

TEST(SetTests, ExecuteSetSideWallTestUpdatesConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "12";
    cmd.tokens[5] = "0.5";

    mock().expectOneCall("save_side_wall_detection_config_as_test")
        .withUnsignedIntParameter("reading_threshold", 10u)
        .withUnsignedIntParameter("slope_threshold", 11u)
        .withUnsignedIntParameter("num_detection_samples", 12u)
        .withDoubleParameter("reading_start_offset", 0.5);

    mock().expectOneCall("set_side_wall_detection_config")
        .withUnsignedIntParameter("reading_threshold", 10u)
        .withUnsignedIntParameter("slope_threshold", 11u)
        .withUnsignedIntParameter("num_detection_samples", 12u)
        .withDoubleParameter("reading_start_offset", 0.5);

    execute_set_side_wall_test(&cmd);
}

TEST(SetTests, ValidateSetSideWallTestAcceptsMaximumValidValues)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "1022";
    cmd.tokens[3] = "1022";
    cmd.tokens[4] = "5";
    cmd.tokens[5] = "0.999";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_side_wall_test(&cmd));
}

TEST(SetTests, ValidateSetSideWallTestRejectsReadingThreshold1023)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "1023";
    cmd.tokens[3] = "10";
    cmd.tokens[4] = "5";
    cmd.tokens[5] = "0.5";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_side_wall_test(&cmd));

    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

TEST(SetTests, ValidateSetSideWallTestRejectsSlopeThreshold1023)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "1023";
    cmd.tokens[4] = "5";
    cmd.tokens[5] = "0.5";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_side_wall_test(&cmd));

    LONGS_EQUAL(3u, cmd.bad_parameter_index);
}

TEST(SetTests, ValidateSetSideWallTestRejectsNegativeReadingStartOffset)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "5";
    cmd.tokens[5] = "-0.1";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_side_wall_test(&cmd));

    LONGS_EQUAL(5u, cmd.bad_parameter_index);
}

TEST(SetTests, ValidateSetSideWallTestRejectsReadingStartOffsetOfOne)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    cmd.tokens[2] = "10";
    cmd.tokens[3] = "11";
    cmd.tokens[4] = "5";
    cmd.tokens[5] = "1.0";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_set_side_wall_test(&cmd));

    LONGS_EQUAL(5u, cmd.bad_parameter_index);
}
