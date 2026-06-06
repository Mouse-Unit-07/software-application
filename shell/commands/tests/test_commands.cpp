/*================================ FILE INFO =================================*/
/* Filename           : test_commands.cpp                                     */
/*                                                                            */
/* Test implementation for commands.c                                         */
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
#include "commands.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
/* none */

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

void print_hardware_state(void)
{
    mock().actualCall("print_hardware_state");
}

uint32_t get_current_global_time_sec(void)
{
    return mock().actualCall("get_current_global_time_sec")
        .returnUnsignedIntValue();
}

void processor_test(void)
{
    mock().actualCall("processor_test");
}

void battery_comparator_test(void)
{
    mock().actualCall("battery_comparator_test");
}

void power_enabler_test(void)
{
    mock().actualCall("power_enabler_test");
}

void led_test(void)
{
    mock().actualCall("led_test");
}

void pushbutton_test(void)
{
    mock().actualCall("pushbutton_test");
}

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

void set_maze_solver_config(struct maze_solver_config cfg)
{
    mock().actualCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", cfg.maze_size);
}

void set_test_maze_solver_config(struct maze_solver_config cfg)
{
    mock().actualCall("set_test_maze_solver_config")
        .withUnsignedIntParameter("maze_size", cfg.maze_size)
        .withUnsignedIntParameter("total_timeout_sec",
                                  cfg.total_timeout_sec)
        .withUnsignedIntParameter("move_forward_time_sec",
                                  cfg.move_forward_time_sec)
        .withUnsignedIntParameter("rotate_90_deg_time_sec",
                                  cfg.rotate_90_deg_time_sec)
        .withUnsignedIntParameter("rotate_180_deg_time_sec",
                                  cfg.rotate_180_deg_time_sec);
}

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
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(CommandsTests, GetCommandTreeRootCountReturnsExpectedValue)
{
    LONGS_EQUAL(8u, get_command_tree_root_count());
}

TEST(CommandsTests, TestCommandContainsFiveSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    struct command_node const *node = find_command_node(&cmd).node;

    LONGS_EQUAL(5u, node->child_count);
}

TEST(CommandsTests, GetCommandTreeRootContainsHelpNode)
{
    struct command_node const *root = get_command_tree_root();

    STRCMP_EQUAL("help", root[0].name);

    STRCMP_EQUAL("Display available commands", root[0].help);

    CHECK(root[0].validate != nullptr);
    CHECK(root[0].execute != nullptr);
}

TEST(CommandsTests, FindCommandNodeReturnsNullForUnknownCommand)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "invalid";

    struct command_match match{find_command_node(&cmd)};

    POINTERS_EQUAL(nullptr, match.node);
}

TEST(CommandsTests, FindCommandNodeReturnsNullForEmptyCommand)
{
    struct command cmd{{0}};

    struct command_match match{find_command_node(&cmd)};

    POINTERS_EQUAL(nullptr, match.node);
}

/*----------------------------------------------------------------------------*/
/* help */
TEST(CommandsTests, FindHelpCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_node const *node{find_command_node(&cmd).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("help", node->name);

    POINTERS_EQUAL((void *)validate_help, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_help, (void *)node->execute);
}

TEST(CommandsTests, HelpCommandMatchDepthIsOne)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_match match{find_command_node(&cmd)};

    LONGS_EQUAL(1u, match.depth);
}

TEST(CommandsTests, ValidateHelpReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_help(&cmd));
}

TEST(CommandsTests, ValidateHelpReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_help(&cmd));
}

/*----------------------------------------------------------------------------*/
/* clear */
TEST(CommandsTests, FindClearCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "clear";

    struct command_node const *node{find_command_node(&cmd).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("clear", node->name);

    POINTERS_EQUAL((void *)validate_clear, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_clear, (void *)node->execute);
}

TEST(CommandsTests, ValidateClearReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_clear(&cmd));
}

TEST(CommandsTests, ValidateClearReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_clear(&cmd));
}

/*----------------------------------------------------------------------------*/
/* faults */
TEST(CommandsTests, FindFaultsCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "faults";

    struct command_node const *node{find_command_node(&cmd).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("faults", node->name);

    POINTERS_EQUAL((void *)validate_hardware_faults, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_hardware_faults, (void *)node->execute);
}

TEST(CommandsTests, ValidateHardwareFaultsReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_hardware_faults(&cmd));
}

TEST(CommandsTests, ValidateHardwareFaultsReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_hardware_faults(&cmd));
}

TEST(CommandsTests, ExecuteHardwareFaultsCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("print_hardware_state");

    execute_hardware_faults(&cmd);
}

/*----------------------------------------------------------------------------*/
/* time */
TEST(CommandsTests, FindTimeCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "time";

    struct command_node const *node{find_command_node(&cmd).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("time", node->name);

    POINTERS_EQUAL((void *)validate_get_time, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_get_time, (void *)node->execute);
}

TEST(CommandsTests, ValidateTimeReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_time(&cmd));
}

TEST(CommandsTests, ValidateTimeReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_time(&cmd));
}

TEST(CommandsTests, ExecuteTimeCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_current_global_time_sec")
        .andReturnValue(0u);

    execute_get_time(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test */
TEST(CommandsTests, RootContainsTestNode)
{
    struct command_node const *root = get_command_tree_root();

    CHECK_TRUE(strcmp(root[4].name, "test") == 0);
}

TEST(CommandsTests, FindCommandNodeReturnsTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("test", node->name);
}

TEST(CommandsTests, ValidateTestProcessorReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test(&cmd));
}

/*----------------------------------------------------------------------------*/
/* test processor */
TEST(CommandsTests, FindCommandNodeReturnsProcessorNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("processor", node->name);
}

TEST(CommandsTests, TestProcessorCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    struct command_match match{find_command_node(&cmd)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, ValidateTestProcessorReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_processor(&cmd));
}

TEST(CommandsTests, ValidateTestProcessorReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";
    cmd.tokens[2] = "extra";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_processor(&cmd));
}

TEST(CommandsTests, ExecuteTestProcessorCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("processor_test");

    execute_test_processor(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test battery */
TEST(CommandsTests, FindCommandNodeReturnsBatteryNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "battery";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("battery", node->name);
}

TEST(CommandsTests, ValidateTestBatteryReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "battery";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_battery(&cmd));
}

TEST(CommandsTests, ExecuteTestBatteryCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("battery_comparator_test");

    execute_test_battery(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test enabler */
TEST(CommandsTests, FindCommandNodeReturnsEnablerNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "enabler";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("enabler", node->name);
}

TEST(CommandsTests, ValidateTestEnablerReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "enabler";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_enabler(&cmd));
}

TEST(CommandsTests, ExecuteTestEnablerCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("power_enabler_test");

    execute_test_enabler(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test led */
TEST(CommandsTests, FindCommandNodeReturnsLedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "led";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("led", node->name);
}

TEST(CommandsTests, ValidateTestLedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "led";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_led(&cmd));
}

TEST(CommandsTests, ExecuteTestLedCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("led_test");

    execute_test_led(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test pushbutton */
TEST(CommandsTests, FindCommandNodeReturnsPushbuttonNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "pushbutton";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("pushbutton", node->name);
}

TEST(CommandsTests, ValidateTestPushbuttonReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "pushbutton";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_pushbutton(&cmd));
}

TEST(CommandsTests, ExecuteTestPushbuttonCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("pushbutton_test");

    execute_test_pushbutton(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get */
TEST(CommandsTests, RootContainsGetNode)
{
    struct command_node const *root = get_command_tree_root();

    CHECK_TRUE(strcmp(root[5].name, "get") == 0);
}

TEST(CommandsTests, FindCommandNodeReturnsGetNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "get";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("get", node->name);
}

TEST(CommandsTests, ValidateGetReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "get";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_get(&cmd));
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
TEST(CommandsTests, FindCommandNodeReturnsGetSolverDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-default", node->name);
}

TEST(CommandsTests, ValidateGetSolverDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_default(&cmd));
}

TEST(CommandsTests, ValidateGetSolverDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_default(&cmd));
}

TEST(CommandsTests, ExecuteGetSolverDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-test */
TEST(CommandsTests, FindCommandNodeReturnsGetSolverTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-test", node->name);
}

TEST(CommandsTests, ValidateGetSolverTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_test(&cmd));
}

TEST(CommandsTests, ValidateGetSolverTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_test(&cmd));
}

TEST(CommandsTests, ExecuteGetSolverTestRuns)
{
    struct command cmd{{0}};

    execute_get_solver_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-current */
TEST(CommandsTests, FindCommandNodeReturnsGetSolverCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-current", node->name);
}

TEST(CommandsTests, ValidateGetSolverCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_solver_current(&cmd));
}

TEST(CommandsTests, ValidateGetSolverCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "solver-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_solver_current(&cmd));
}

TEST(CommandsTests, ExecuteGetSolverCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_solver_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set */
TEST(CommandsTests, RootContainsSetNode)
{
    struct command_node const *root = get_command_tree_root();

    CHECK_TRUE(strcmp(root[6].name, "set") == 0);
}

TEST(CommandsTests, FindCommandNodeReturnsSetNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "set";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("set", node->name);
}

TEST(CommandsTests, ValidateSetReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "set";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set(&cmd));
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
TEST(CommandsTests, FindCommandNodeReturnsSolverDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = "solver-default";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-default", node->name);
}

TEST(CommandsTests, ValidateSetSolverDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_default(&cmd));
}

TEST(CommandsTests, ExecuteSetSolverDefaultCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 16u);

    execute_set_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* set solver-test */
TEST(CommandsTests, FindCommandNodeReturnsSolverTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "set";
    cmd.tokens[1] = "solver-test";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solver-test", node->name);
}

TEST(CommandsTests, ValidateSetSolverTestReturnsSuccessForDefaultConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_set_solver_test(&cmd));
}

TEST(CommandsTests, ValidateSetSolverTestReturnsSuccessForEditConfig)
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

TEST(CommandsTests, ValidateSetSolverTestReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_set_solver_test(&cmd));
}

TEST(CommandsTests, ValidateSetSolverTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 8;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_set_solver_test(&cmd));
}

TEST(CommandsTests, ValidateSetSolverTestReturnsBadParameter)
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

TEST(CommandsTests, ExecuteSetSolverTestUsesStoredTestConfig)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    mock().expectOneCall("set_maze_solver_config")
        .withUnsignedIntParameter("maze_size", 8u);

    execute_set_solver_test(&cmd);
}

TEST(CommandsTests, ExecuteSetSolverTestUpdatesTestConfig)
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

/*----------------------------------------------------------------------------*/
/* solve */
TEST(CommandsTests, RootContainsSolveNode)
{
    struct command_node const *root = get_command_tree_root();

    CHECK_TRUE(strcmp(root[7].name, "solve") == 0);
}

TEST(CommandsTests, SolveCommandContainsTwoSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "solve";

    struct command_node const *node = find_command_node(&cmd).node;

    LONGS_EQUAL(2u, node->child_count);
}

TEST(CommandsTests, FindCommandNodeReturnsSolveNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "solve";

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("solve", node->name);
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

    struct command_node const *node = find_command_node(&cmd).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("wallfollower", node->name);
}

TEST(CommandsTests, WallFollowerCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    cmd.tokens[0] = "solve";
    cmd.tokens[1] = "wallfollower";

    struct command_match match = find_command_node(&cmd);

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

    struct command_node const *node = find_command_node(&cmd).node;

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
