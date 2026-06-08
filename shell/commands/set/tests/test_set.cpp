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
    fake_root_commands[0].execute = NULL;
    fake_root_commands[0].children = get_set_commands();
    fake_root_commands[0].child_count = get_set_commands_count();
}

void deinit_test_fakes(void)
{
    fake_root_commands[0] = {};
}

struct command make_get_command(char const *subcommand, uint32_t token_count)
{
    struct command cmd{{0}};

    cmd.token_count = token_count;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = subcommand;

    return cmd;
}

void check_command_lookup(char const *command_name, uint32_t token_count)
{
    struct command cmd{make_get_command(command_name, token_count)};

    struct command_node const *node = 
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL(command_name, node->name);
}

void check_validation_success(char const *command_name, uint32_t token_count,
                              enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_get_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate(&cmd));
}

void check_validation_too_many_params(char const *command_name, uint32_t token_count,
                                      enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_get_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate(&cmd));
}

void check_validation_too_few_params(char const *command_name, uint32_t token_count,
                                      enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_get_command(command_name, token_count)};

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate(&cmd));
}

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
    CHECK(node->validate != nullptr);
    POINTERS_EQUAL(get_set_commands(), node->children);
    LONGS_EQUAL(get_set_commands_count(), node->child_count);
}

TEST(SetTests, SetCommandsAreConfiguredCorrectly)
{
    static const char *expected_names[] =
    {
        "solver-default",
        "solver-test",
        "mouse-physical-default",
        "mouse-physical-test",
        "maze-physical-default",
        "maze-physical-test"
    };

    const struct command_node *commands = get_set_commands();

    LONGS_EQUAL(sizeof(expected_names) / sizeof(expected_names[0]), get_set_commands_count());

    for (uint32_t i{0}; i < get_set_commands_count(); i++) {
        STRCMP_EQUAL(expected_names[i], commands[i].name);
        CHECK(commands[i].validate != nullptr);
        CHECK(commands[i].execute != nullptr);
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

TEST(SetTests, ValidateSetReturnsTooFewParameters)
{
    check_validation_too_few_params("", 1, validate_set);
}

TEST(SetTests, ValidateSetReturnsSuccess)
{
    check_validation_success("", 2, validate_set);
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
TEST(SetTests, FindCommandNodeReturnsSolverDefaultNode)
{
    check_command_lookup("solver-default", 2);
}

TEST(SetTests, ValidateSetSolverDefaultReturnsSuccess)
{
    check_validation_success("solver-default", 2, validate_set_solver_default);
}

TEST(SetTests, ValidateSetSolverDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("solver-default", 3, validate_set_solver_default);
}

TEST(SetTests, ExecuteSetSolverDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 16u);

    execute_set_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set solver-test */
TEST(SetTests, FindCommandNodeReturnsSolverTestNode)
{
    check_command_lookup("solver-test", 2);
}

TEST(SetTests, ValidateSetSolverTestReturnsSuccessForDefaultConfig)
{
    check_validation_success("solver-test", 2, validate_set_solver_test);
}

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

TEST(SetTests, ValidateSetSolverTestReturnsTooFewParameters)
{
    check_validation_too_few_params("solver-test", 6, validate_set_solver_test);
}

TEST(SetTests, ValidateSetSolverTestReturnsTooManyParameters)
{
    check_validation_too_many_params("solver-test", 8, validate_set_solver_test);
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
TEST(SetTests, FindCommandNodeReturnsMousePhysicalDefaultNode)
{
    check_command_lookup("mouse-physical-default", 2);
}

TEST(SetTests, ValidateSetMousePhysicalDefaultReturnsSuccess)
{
    check_validation_success("mouse-physical-default", 2, validate_set_mouse_physical_default);
}

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
TEST(SetTests, FindCommandNodeReturnsMousePhysicalTestNode)
{
    check_command_lookup("mouse-physical-test", 2);
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsSuccessForStoredConfig)
{
    check_validation_success("mouse-physical-test", 2, validate_set_mouse_physical_test);
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsSuccessForEditConfig)
{
    check_validation_success("mouse-physical-test", 8, validate_set_mouse_physical_test);
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsTooFewParameters)
{
    check_validation_too_few_params("mouse-physical-test", 6, validate_set_mouse_physical_test);
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsTooManyParameters)
{
    check_validation_too_many_params("mouse-physical-test", 9, validate_set_mouse_physical_test);
}

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
TEST(SetTests, FindCommandNodeReturnsMazePhysicalDefaultNode)
{
    check_command_lookup("maze-physical-default", 2);
}

TEST(SetTests, ValidateSetMazePhysicalDefaultReturnsSuccess)
{
    check_validation_success("maze-physical-default", 2, validate_set_maze_physical_default);
}

TEST(SetTests, ValidateSetMazePhysicalDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-physical-default", 3, validate_set_maze_physical_default);
}

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
TEST(SetTests, FindCommandNodeReturnsMazePhysicalTestNode)
{
    check_command_lookup("maze-physical-test", 2);
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsSuccessForStoredConfig)
{
    check_validation_success("maze-physical-test", 2, validate_set_maze_physical_test);
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsSuccessForEditConfig)
{
    check_validation_success("maze-physical-test", 4, validate_set_maze_physical_test);
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsTooFewParameters)
{
    check_validation_too_few_params("mouse-physical-test", 3, validate_set_maze_physical_test);
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-physical-test", 5, validate_set_maze_physical_test);
}

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
