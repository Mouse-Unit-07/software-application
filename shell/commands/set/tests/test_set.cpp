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

struct maze_solver_config get_default_maze_solver_config(void)
{
    struct maze_solver_config cfg{};
    cfg.maze_size = 16u;
    return cfg;
}

struct maze_solver_config get_test_maze_solver_config(void)
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

void set_test_maze_solver_config(struct maze_solver_config cfg)
{
    mock().actualCall("set_test_maze_solver_config")
        .withUnsignedIntParameter("maze_size", cfg.maze_size)
        .withUnsignedIntParameter("total_timeout_sec", cfg.total_timeout_sec)
        .withUnsignedIntParameter("move_forward_time_sec", cfg.move_forward_time_sec)
        .withUnsignedIntParameter("rotate_90_deg_time_sec", cfg.rotate_90_deg_time_sec)
        .withUnsignedIntParameter("rotate_180_deg_time_sec", cfg.rotate_180_deg_time_sec);
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
}

TEST(SetTests, GetSetCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(2u, get_set_commands_count());
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

    mock().expectOneCall("set_test_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 16u)
        .withUnsignedIntParameter("total_timeout_sec", 100u)
        .withUnsignedIntParameter("move_forward_time_sec", 10u)
        .withUnsignedIntParameter("rotate_90_deg_time_sec", 20u)
        .withUnsignedIntParameter("rotate_180_deg_time_sec", 40u);

    execute_set_solver_test(&cmd);
}
