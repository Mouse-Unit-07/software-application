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

TEST(SetTests, SetCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("solver-default", commands[0].name);
    STRCMP_EQUAL("solver-test", commands[1].name);
    STRCMP_EQUAL("mouse-physical-default", commands[2].name);
    STRCMP_EQUAL("mouse-physical-test", commands[3].name);
    STRCMP_EQUAL("maze-physical-default", commands[4].name);
    STRCMP_EQUAL("maze-physical-test", commands[5].name);
}

TEST(SetTests, GetSetCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(6u, get_set_commands_count());
}

TEST(SetTests, GetSetCommandsContainsSolverDefaultNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("solver-default", commands[0].name);

    CHECK(commands[0].validate != nullptr);
    CHECK(commands[0].execute != nullptr);
}

TEST(SetTests, GetSetCommandsContainsSolverTestNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("solver-test", commands[1].name);

    CHECK(commands[1].validate != nullptr);
    CHECK(commands[1].execute != nullptr);
}

TEST(SetTests, GetSetCommandsContainsMousePhysicalDefaultNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("mouse-physical-default", commands[2].name);

    CHECK(commands[2].validate != nullptr);
    CHECK(commands[2].execute != nullptr);
}

TEST(SetTests, GetSetCommandsContainsMousePhysicalTestNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("mouse-physical-test", commands[3].name);

    CHECK(commands[3].validate != nullptr);
    CHECK(commands[3].execute != nullptr);
}

TEST(SetTests, GetSetCommandsContainsMazePhysicalDefaultNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("maze-physical-default", commands[4].name);

    CHECK(commands[4].validate != nullptr);
    CHECK(commands[4].execute != nullptr);
}

TEST(SetTests, GetSetCommandsContainsMazePhysicalTestNode)
{
    const struct command_node *commands = get_set_commands();

    STRCMP_EQUAL("maze-physical-test", commands[5].name);

    CHECK(commands[5].validate != nullptr);
    CHECK(commands[5].execute != nullptr);
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
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "set";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set(&cmd));
}

TEST(SetTests, ValidateSetReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set(&cmd));
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
TEST(SetTests, FindCommandNodeReturnsSolverDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = "solver-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-default", node->name);
}

TEST(SetTests, ValidateSetSolverDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_default(&cmd));
}

TEST(SetTests, ValidateSetSolverDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set_solver_default(&cmd));
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
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = "solver-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-test", node->name);
}

TEST(SetTests, ValidateSetSolverTestReturnsSuccessForDefaultConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_test(&cmd));
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
    struct command cmd{{0}};
    cmd.token_count = 6;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set_solver_test(&cmd));
}

TEST(SetTests, ValidateSetSolverTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 8;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set_solver_test(&cmd));
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
TEST(SetTests, ValidateSetMousePhysicalDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_mouse_physical_default(&cmd));
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
TEST(SetTests, ValidateSetMousePhysicalTestReturnsSuccessForStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_mouse_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsSuccessForEditConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 8;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_mouse_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set_mouse_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMousePhysicalTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 9;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set_mouse_physical_test(&cmd));
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
TEST(SetTests, ValidateSetMazePhysicalDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_maze_physical_default(&cmd));
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
TEST(SetTests, ValidateSetMazePhysicalTestReturnsSuccessForStoredConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_maze_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsSuccessForEditConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_maze_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set_maze_physical_test(&cmd));
}

TEST(SetTests, ValidateSetMazePhysicalTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 5;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set_maze_physical_test(&cmd));
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
