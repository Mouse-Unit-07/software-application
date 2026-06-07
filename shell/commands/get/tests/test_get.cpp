/*================================ FILE INFO =================================*/
/* Filename           : test_get.cpp                                          */
/*                                                                            */
/* Test implementation for get.c                                              */
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
#include "command.h"
#include "get.h"

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
    fake_root_commands[0].name = "get";
    fake_root_commands[0].help = "Read configuration values";
    fake_root_commands[0].validate = validate_get;
    fake_root_commands[0].execute = NULL;
    fake_root_commands[0].children = get_get_commands();
    fake_root_commands[0].child_count = get_get_commands_count();
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

struct maze_solver_config get_maze_solver_config(void)
{
    struct maze_solver_config cfg{};
    return cfg;
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(GetTests)
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
TEST(GetTests, GetGetNodeReturnsExpectedNode)
{
    const struct command_node *node = get_get_node();

    CHECK(node != nullptr);
    STRCMP_EQUAL("get", node->name);
    CHECK(node->validate != nullptr);
    POINTERS_EQUAL(get_get_commands(), node->children);
    LONGS_EQUAL(get_get_commands_count(), node->child_count);
}

TEST(GetTests, GetGetCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(3u, get_get_commands_count());
}

TEST(GetTests, GetGetCommandsContainsSolverDefaultNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("solver-default", commands[0].name);

    CHECK(commands[0].validate != nullptr);
    CHECK(commands[0].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsSolverTestNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("solver-test", commands[1].name);

    CHECK(commands[1].validate != nullptr);
    CHECK(commands[1].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsSolverCurrentNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("solver-current", commands[2].name);

    CHECK(commands[2].validate != nullptr);
    CHECK(commands[2].execute != nullptr);
}

TEST(GetTests, GetCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("solver-default", commands[0].name);
    STRCMP_EQUAL("solver-test", commands[1].name);
    STRCMP_EQUAL("solver-current", commands[2].name);
}

/*----------------------------------------------------------------------------*/
/* get */
TEST(GetTests, FindCommandNodeReturnsGetNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "get";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("get", node->name);
}

TEST(GetTests, ValidateGetReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get(&cmd));
}

TEST(GetTests, ValidateGetReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "get";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_get(&cmd));
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
TEST(GetTests, FindCommandNodeReturnsGetSolverDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-default", node->name);
}

TEST(GetTests, ValidateGetSolverDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_default(&cmd));
}

TEST(GetTests, ValidateGetSolverDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_default(&cmd));
}

TEST(GetTests, ExecuteGetSolverDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-test */
TEST(GetTests, FindCommandNodeReturnsGetSolverTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-test", node->name);
}

TEST(GetTests, ValidateGetSolverTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_test(&cmd));
}

TEST(GetTests, ValidateGetSolverTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_test(&cmd));
}

TEST(GetTests, ExecuteGetSolverTestRuns)
{
    struct command cmd{{0}};

    execute_get_solver_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-current */
TEST(GetTests, FindCommandNodeReturnsGetSolverCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-current", node->name);
}

TEST(GetTests, ValidateGetSolverCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_current(&cmd));
}

TEST(GetTests, ValidateGetSolverCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_current(&cmd));
}

TEST(GetTests, ExecuteGetSolverCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_solver_current(&cmd);
}
