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
#include "navigation.h"
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

struct maze_solver_config get_maze_solver_config(void)
{
    struct maze_solver_config cfg{};
    return cfg;
}

struct mouse_physical_params get_mouse_physical_params(void)
{
    struct mouse_physical_params p{};
    p.wheel_diameter_mm = 32.0;
    return p;
}

struct mouse_calculated_params get_mouse_calculated_params(void)
{
    struct mouse_calculated_params p{};
    p.gear_ratio = 3.0;
    return p;
}

struct maze_physical_params get_maze_physical_params(void)
{
    struct maze_physical_params p{};
    p.post_size_mm = 12.0;
    return p;
}

struct maze_calculated_params get_maze_calculated_params(void)
{
    struct maze_calculated_params p{};
    p.cell_size_mm = 168.0;
    return p;
}

struct navigation_params get_navigation_params(void)
{
    struct navigation_params p{};
    p.move_forward_one_cell_target_ticks = 1000;
    return p;
}

struct mouse_physical_params get_saved_default_mouse_physical_params(void)
{
    struct mouse_physical_params p{};
    p.wheel_diameter_mm = 32.0;
    return p;
}

struct mouse_physical_params get_saved_test_mouse_physical_params(void)
{
    struct mouse_physical_params p{};
    p.wheel_diameter_mm = 28.0;
    return p;
}

struct maze_physical_params get_saved_default_maze_physical_params(void)
{
    struct maze_physical_params p{};
    p.post_size_mm = 12.0;
    return p;
}

struct maze_physical_params get_saved_test_maze_physical_params(void)
{
    struct maze_physical_params p{};
    p.post_size_mm = 10.0;
    return p;
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
    LONGS_EQUAL(12u, get_get_commands_count());
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

TEST(GetTests, GetGetCommandsContainsMousePhysicalDefaultNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("mouse-physical-default", commands[3].name);

    CHECK(commands[3].validate != nullptr);
    CHECK(commands[3].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMousePhysicalTestNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("mouse-physical-test", commands[4].name);

    CHECK(commands[4].validate != nullptr);
    CHECK(commands[4].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMousePhysicalCurrentNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("mouse-physical-current", commands[5].name);

    CHECK(commands[5].validate != nullptr);
    CHECK(commands[5].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMazePhysicalDefaultNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("maze-physical-default", commands[7].name);

    CHECK(commands[7].validate != nullptr);
    CHECK(commands[7].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMazePhysicalTestNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("maze-physical-test", commands[8].name);

    CHECK(commands[8].validate != nullptr);
    CHECK(commands[8].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMazePhysicalCurrentNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("maze-physical-current", commands[9].name);

    CHECK(commands[9].validate != nullptr);
    CHECK(commands[9].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMouseCalculatedNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("mouse-calculated", commands[6].name);

    CHECK(commands[6].validate != nullptr);
    CHECK(commands[6].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsMazeCalculatedNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("maze-calculated", commands[10].name);

    CHECK(commands[10].validate != nullptr);
    CHECK(commands[10].execute != nullptr);
}

TEST(GetTests, GetGetCommandsContainsNavigationNode)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("navigation", commands[11].name);

    CHECK(commands[11].validate != nullptr);
    CHECK(commands[11].execute != nullptr);
}

TEST(GetTests, GetCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_get_commands();

    STRCMP_EQUAL("solver-default", commands[0].name);
    STRCMP_EQUAL("solver-test", commands[1].name);
    STRCMP_EQUAL("solver-current", commands[2].name);
    STRCMP_EQUAL("mouse-physical-default", commands[3].name);
    STRCMP_EQUAL("mouse-physical-test", commands[4].name);
    STRCMP_EQUAL("mouse-physical-current", commands[5].name);
    STRCMP_EQUAL("mouse-calculated", commands[6].name);
    STRCMP_EQUAL("maze-physical-default", commands[7].name);
    STRCMP_EQUAL("maze-physical-test", commands[8].name);
    STRCMP_EQUAL("maze-physical-current", commands[9].name);
    STRCMP_EQUAL("maze-calculated", commands[10].name);
    STRCMP_EQUAL("navigation", commands[11].name);
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

/*----------------------------------------------------------------------------*/
/* get mouse-physical-default */
TEST(GetTests, FindCommandNodeReturnsGetMousePhysicalDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("mouse-physical-default", node->name);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_mouse_physical_default(&cmd));
}

TEST(GetTests, ValidateGetMousePhysicalDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_mouse_physical_default(&cmd));
}

TEST(GetTests, ExecuteGetMousePhysicalDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_mouse_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-test */
TEST(GetTests, FindCommandNodeReturnsGetMousePhysicalTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("mouse-physical-test", node->name);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_mouse_physical_test(&cmd));
}

TEST(GetTests, ValidateGetMousePhysicalDefaultTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_mouse_physical_test(&cmd));
}

TEST(GetTests, ExecuteGetMousePhysicalDefaultTestRuns)
{
    struct command cmd{{0}};

    execute_get_mouse_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-current */
TEST(GetTests, FindCommandNodeReturnsGetMousePhysicalCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("mouse-physical-current", node->name);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_mouse_physical_current(&cmd));
}

TEST(GetTests, ValidateGetMousePhysicalDefaultCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-physical-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_mouse_physical_current(&cmd));
}

TEST(GetTests, ExecuteGetMousePhysicalDefaultCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_mouse_physical_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-calculated */
TEST(GetTests, FindCommandNodeReturnsGetMouseCalculatedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-calculated";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("mouse-calculated", node->name);
}

TEST(GetTests, ValidateGetMouseCalculatedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_mouse_calculated(&cmd));
}

TEST(GetTests, ValidateGetMouseCalculatedReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "mouse-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_mouse_calculated(&cmd));
}

TEST(GetTests, ExecuteGetMouseCalculatedRuns)
{
    struct command cmd{{0}};

    execute_get_mouse_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-default */
TEST(GetTests, FindCommandNodeReturnsGetMazePhysicalDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("maze-physical-default", node->name);
}

TEST(GetTests, ValidateGetMazePhysicalDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_maze_physical_default(&cmd));
}

TEST(GetTests, ValidateGetMazePhysicalDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_maze_physical_default(&cmd));
}

TEST(GetTests, ExecuteGetMazePhysicalDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_maze_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-test */
TEST(GetTests, FindCommandNodeReturnsGetMazePhysicalTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("maze-physical-test", node->name);
}

TEST(GetTests, ValidateGetMazePhysicalTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_maze_physical_test(&cmd));
}

TEST(GetTests, ValidateGetMazePhysicalTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_maze_physical_test(&cmd));
}

TEST(GetTests, ExecuteGetMazePhysicalTestRuns)
{
    struct command cmd{{0}};

    execute_get_maze_physical_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-current */
TEST(GetTests, FindCommandNodeReturnsGetMazePhysicalCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("maze-physical-current", node->name);
}

TEST(GetTests, ValidateGetMazePhysicalCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_maze_physical_current(&cmd));
}

TEST(GetTests, ValidateGetMazePhysicalCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-physical-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_maze_physical_current(&cmd));
}

TEST(GetTests, ExecuteGetMazePhysicalCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_maze_physical_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-calculated */
TEST(GetTests, FindCommandNodeReturnsGetMazeCalculatedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-calculated";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("maze-calculated", node->name);
}

TEST(GetTests, ValidateGetMazeCalculatedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_maze_calculated(&cmd));
}

TEST(GetTests, ValidateGetMazeCalculatedReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "maze-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_maze_calculated(&cmd));
}

TEST(GetTests, ExecuteGetMazeCalculatedRuns)
{
    struct command cmd{{0}};

    execute_get_maze_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get navigation */
TEST(GetTests, FindCommandNodeReturnsGetNavigationNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "navigation";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("navigation", node->name);
}

TEST(GetTests, ValidateGetNavigationReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "navigation";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_navigation(&cmd));
}

TEST(GetTests, ValidateGetNavigationReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "navigation";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_navigation(&cmd));
}

TEST(GetTests, ExecuteGetNavigationRuns)
{
    struct command cmd{{0}};

    execute_get_navigation(&cmd);
}
