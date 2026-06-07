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
#include "partial_flood_fill.h"
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
    fake_root_commands[0].execute = NULL;
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

void run_partial_flood_fill(bool enable_print)
{
    mock().actualCall("run_partial_flood_fill")
        .withBoolParameter("enable_print", enable_print);
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(CommandsTests)
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
TEST(CommandsTests, GetSolveNodeReturnsValidNode)
{
    const struct command_node *node = get_solve_node();

    CHECK(node != nullptr);
    STRCMP_EQUAL("solve", node->name);
    CHECK(node->validate != nullptr);
    CHECK(node->execute == nullptr);
    POINTERS_EQUAL(get_solve_commands(), node->children);
    LONGS_EQUAL(get_solve_commands_count(), node->child_count);
}

TEST(CommandsTests, GetSolveCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(2u, get_solve_commands_count());
}

TEST(CommandsTests, SolveCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_solve_commands();

    STRCMP_EQUAL("wallfollower", commands[0].name);
    STRCMP_EQUAL("floodfill", commands[1].name);
}

TEST(CommandsTests, GetSolveCommandsContainsWallFollowerNode)
{
    const struct command_node *commands = get_solve_commands();

    STRCMP_EQUAL("wallfollower", commands[0].name);

    CHECK(commands[0].validate != nullptr);
    CHECK(commands[0].execute != nullptr);
}

TEST(CommandsTests, GetSolveCommandsContainsFloodFillNode)
{
    const struct command_node *commands = get_solve_commands();

    STRCMP_EQUAL("floodfill", commands[1].name);

    CHECK(commands[1].validate != nullptr);
    CHECK(commands[1].execute != nullptr);
}

/*----------------------------------------------------------------------------*/
/* solve */
TEST(CommandsTests, SolveCommandContainsTwoSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "solve";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(2u, node->child_count);
}

TEST(CommandsTests, FindCommandNodeReturnsSolveNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "solve";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solve", node->name);
}

TEST(CommandsTests, ValidateSolveReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve(&cmd));
}

TEST(CommandsTests, ValidateSolveReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "solve";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_solve(&cmd));
}

/*----------------------------------------------------------------------------*/
/* solve wallfollower */
TEST(CommandsTests, FindCommandNodeReturnsWallFollowerNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("wallfollower", node->name);
}

TEST(CommandsTests, WallFollowerCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";

    struct command_match match =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT);

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, ValidateSolveWallFollowerLeftReturnsSuccess)
{
    struct command cmd{{0}};

    cmd.token_count = 3;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";
    cmd.tokens[2] = "left";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_wallfollower(&cmd));
}

TEST(CommandsTests, ValidateSolveWallFollowerRightEnableReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 4;
    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";
    cmd.tokens[2] = "right";
    cmd.tokens[3] = "enable";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_wallfollower(&cmd));
}

TEST(CommandsTests, ValidateSolveWallFollowerReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_solve_wallfollower(&cmd));
}

TEST(CommandsTests, ValidateSolveWallFollowerReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 5;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_solve_wallfollower(&cmd));
}

TEST(CommandsTests, ValidateSolveWallFollowerRejectsBadMode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[2] = "bad";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_solve_wallfollower(&cmd));

    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

TEST(CommandsTests, ValidateSolveWallFollowerRejectsBadPrintOption)
{
    struct command cmd{{0}};
    cmd.token_count = 4;
    cmd.tokens[2] = "left";
    cmd.tokens[3] = "bad";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_solve_wallfollower(&cmd));

    LONGS_EQUAL(3u, cmd.bad_parameter_index);
}

TEST(CommandsTests, ExecuteSolveWallFollowerLeft)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[2] = "left";

    mock().expectOneCall("run_wall_follower")
        .withUnsignedIntParameter("mode", WALL_FOLLOWER_LEFT)
        .withBoolParameter("enable_print", false);

    execute_solve_wallfollower(&cmd);
}

TEST(CommandsTests, ExecuteSolveWallFollowerRightEnable)
{
    struct command cmd{{0}};
    cmd.token_count = 4;
    cmd.tokens[2] = "right";
    cmd.tokens[3] = "enable";

    mock().expectOneCall("run_wall_follower")
        .withUnsignedIntParameter("mode", WALL_FOLLOWER_RIGHT)
        .withBoolParameter("enable_print", true);

    execute_solve_wallfollower(&cmd);
}

/*----------------------------------------------------------------------------*/
/* solve floodfill */
TEST(CommandsTests, FindCommandNodeReturnsFloodFillNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "floodfill";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("floodfill", node->name);
}

TEST(CommandsTests, ValidateSolveFloodFillReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_floodfill(&cmd));
}

TEST(CommandsTests, ValidateSolveFloodFillEnableReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[2] = "enable";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_solve_floodfill(&cmd));
}

TEST(CommandsTests, ValidateSolveFloodFillReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_solve_floodfill(&cmd));
}

TEST(CommandsTests, ValidateSolveFloodFillRejectsBadParameter)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[2] = "bad";

    LONGS_EQUAL(COMMAND_VALIDATION_BAD_PARAMETER, validate_solve_floodfill(&cmd));

    LONGS_EQUAL(2u, cmd.bad_parameter_index);
}

TEST(CommandsTests, ExecuteSolveFloodFillWithoutPrint)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("run_partial_flood_fill")
        .withBoolParameter("enable_print", false);

    execute_solve_floodfill(&cmd);
}

TEST(CommandsTests, ExecuteSolveFloodFillWithPrint)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[2] = "enable";

    mock().expectOneCall("run_partial_flood_fill")
        .withBoolParameter("enable_print", true);

    execute_solve_floodfill(&cmd);
}
