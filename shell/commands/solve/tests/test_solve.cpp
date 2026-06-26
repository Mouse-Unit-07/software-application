/*================================ FILE INFO =================================*/
/* Filename           : test_solve.cpp                                        */
/*                                                                            */
/* Test implementation for solve.c                                            */
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
#include "wall_follower.h"
#include "command.h"
#include "solve.h"

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
    fake_root_commands[0].name = "solve";
    fake_root_commands[0].help = "Run maze solving algorithms";
    fake_root_commands[0].validate = validate_solve;
    fake_root_commands[0].execute = execute_solve;
    fake_root_commands[0].children = get_solve_commands();
    fake_root_commands[0].child_count = get_solve_commands_count();
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

void run_wall_follower(enum wall_follower_mode mode, bool enable_print)
{
    mock().actualCall("run_wall_follower")
            .withUnsignedIntParameter("mode", (uint32_t)mode)
            .withBoolParameter("enable_print", enable_print);
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(SolveTests)
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
TEST(SolveTests, GetSolveNodeReturnsValidNode)
{
    const struct command_node *node{get_solve_node()};

    CHECK(node != nullptr);
    STRCMP_EQUAL("solve", node->name);
    POINTERS_EQUAL(validate_solve, node->validate);
    POINTERS_EQUAL(execute_solve, node->execute);
    POINTERS_EQUAL(get_solve_commands(), node->children);
    LONGS_EQUAL(get_solve_commands_count(), node->child_count);
}

TEST(SolveTests, GetSolveCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(1u, get_solve_commands_count());
}

TEST(SolveTests, SolveCommandsAreInExpectedOrder)
{
    const struct command_node *commands{get_solve_commands()};

    STRCMP_EQUAL("wallfollower", commands[0].name);
}

TEST(SolveTests, GetSolveCommandsContainsWallFollowerNode)
{
    const struct command_node *commands{get_solve_commands()};

    STRCMP_EQUAL("wallfollower", commands[0].name);

    CHECK(commands[0].validate != nullptr);
    CHECK(commands[0].execute != nullptr);
}

/*----------------------------------------------------------------------------*/
/* solve */
TEST(SolveTests, SolveCommandContainsTwoSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1u;
    cmd.tokens[0] = "solve";

    struct command_node const *node{
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node};

    CHECK(node != nullptr);
    LONGS_EQUAL(1u, node->child_count);
}

TEST(SolveTests, FindCommandNodeReturnsSolveNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1u;
    cmd.tokens[0] = "solve";

    struct command_node const *node{
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node};

    CHECK(node != nullptr);
    STRCMP_EQUAL("solve", node->name);
}

TEST(SolveTests, ValidateSolveReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve(&cmd));
}

TEST(SolveTests, ValidateSolveReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2u;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_solve(&cmd));
}

TEST(SolveTests, ExecuteSolveRunsWithoutCrash)
{
    struct command cmd{{0}};

    execute_solve(&cmd);
}

/*----------------------------------------------------------------------------*/
/* solve wallfollower */
TEST(SolveTests, FindCommandNodeReturnsWallFollowerNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2u;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";

    struct command_node const *node{
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node};

    CHECK(node != nullptr);
    STRCMP_EQUAL("wallfollower", node->name);
}

TEST(SolveTests, WallFollowerCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2u;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";

    struct command_match match{
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(SolveTests, ValidateSolveWallFollowerLeftReturnsSuccess)
{
    struct command cmd{{0}};

    cmd.token_count = 3u;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";
    cmd.tokens[2] = "left";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_wallfollower(&cmd));
}

TEST(SolveTests, ValidateSolveWallFollowerRightEnableReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 4u;
    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";
    cmd.tokens[2] = "right";
    cmd.tokens[3] = "enable";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_wallfollower(&cmd));
}

TEST(SolveTests, ValidateSolveWallFollowerReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2u;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_solve_wallfollower(&cmd));
}

TEST(SolveTests, ValidateSolveWallFollowerReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 5u;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_solve_wallfollower(&cmd));
}

TEST(SolveTests, ValidateSolveWallFollowerRejectsBadMode)
{
    struct command cmd{{0}};
    cmd.token_count = 3u;
    cmd.tokens[2] = "bad";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_solve_wallfollower(&cmd));
    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

TEST(SolveTests, ValidateSolveWallFollowerRejectsBadPrintOption)
{
    struct command cmd{{0}};
    cmd.token_count = 4u;
    cmd.tokens[2] = "left";
    cmd.tokens[3] = "bad";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_solve_wallfollower(&cmd));
    LONGS_EQUAL(3u, cmd.bad_parameter_index);
}

TEST(SolveTests, ExecuteSolveWallFollowerLeft)
{
    struct command cmd{{0}};
    cmd.token_count = 3u;
    cmd.tokens[2] = "left";

    mock().expectOneCall("run_wall_follower")
            .withUnsignedIntParameter("mode", WALL_FOLLOWER_LEFT)
            .withBoolParameter("enable_print", false);

    execute_solve_wallfollower(&cmd);
}

TEST(SolveTests, ExecuteSolveWallFollowerRightEnable)
{
    struct command cmd{{0}};
    cmd.token_count = 4u;
    cmd.tokens[2] = "right";
    cmd.tokens[3] = "enable";

    mock().expectOneCall("run_wall_follower")
            .withUnsignedIntParameter("mode", WALL_FOLLOWER_RIGHT)
            .withBoolParameter("enable_print", true);

    execute_solve_wallfollower(&cmd);
}
