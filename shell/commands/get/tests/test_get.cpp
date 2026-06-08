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

struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 140;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_no_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 141;
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 142;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_one_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 143;
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 144;
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_both_wall_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 145;
    return cfg;
}

struct move_forward_control_config get_no_wall_move_forward_control_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 146;
    return cfg;
}

struct move_forward_control_config get_one_wall_move_forward_control_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 147;
    return cfg;
}

struct move_forward_control_config get_both_wall_move_forward_control_config(void)
{
    struct move_forward_control_config cfg{};
    cfg.base_speed = 148;
    return cfg;
}

struct rotate_control_config get_saved_default_rotate_control_config(void)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 150;
    return cfg;
}

struct rotate_control_config get_saved_test_rotate_control_config(void)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 151;
    return cfg;
}

struct rotate_control_config get_rotate_control_config(void)
{
    struct rotate_control_config cfg{};
    cfg.base_speed = 152;
    return cfg;
}

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 160;
    return cfg;
}

struct front_wall_detection_config get_saved_test_front_wall_detection_config(void)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 161;
    return cfg;
}

struct front_wall_detection_config get_front_wall_detection_config(void)
{
    struct front_wall_detection_config cfg{};
    cfg.reading_threshold = 162;
    return cfg;
}

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 49;
    return cfg;
}

struct side_wall_detection_config get_saved_test_side_wall_detection_config(void)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 50;
    return cfg;
}

struct side_wall_detection_config get_side_wall_detection_config(void)
{
    struct side_wall_detection_config cfg{};
    cfg.reading_threshold = 51;
    return cfg;
}

struct side_wall_calculated_params get_side_wall_calculated_params(void)
{
    struct side_wall_calculated_params p{};
    p.reading_start_offset_ticks = 1;
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
    LONGS_EQUAL(31u, get_get_commands_count());
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

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-default */
TEST(GetTests, FindCommandNodeReturnsMoveForwardNoWallDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-no-wall-default", node->name);
}

TEST(GetTests, ValidateMoveForwardNoWallDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_no_wall_default(&cmd));
}

TEST(GetTests, ValidateMoveForwardNoWallDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_no_wall_default(&cmd));
}

TEST(GetTests, ExecuteMoveForwardNoWallDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_no_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-test */
TEST(GetTests, FindCommandNodeReturnsMoveForwardNoWallTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-no-wall-test", node->name);
}

TEST(GetTests, ValidateMoveForwardNoWallTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_no_wall_test(&cmd));
}

TEST(GetTests, ValidateMoveForwardNoWallTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_no_wall_test(&cmd));
}

TEST(GetTests, ExecuteMoveForwardNoWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_no_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-current */
TEST(GetTests, FindCommandNodeReturnsMoveForwardNoWallCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-no-wall-current", node->name);
}

TEST(GetTests, ValidateMoveForwardNoWallCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_no_wall_current(&cmd));
}

TEST(GetTests, ValidateMoveForwardNoWallCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-no-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_no_wall_current(&cmd));
}

TEST(GetTests, ExecuteMoveForwardNoWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_no_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-default */
TEST(GetTests, FindCommandNodeReturnsMoveForwardOneWallDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-one-wall-default", node->name);
}

TEST(GetTests, ValidateMoveForwardOneWallDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_one_wall_default(&cmd));
}

TEST(GetTests, ValidateMoveForwardOneWallDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_one_wall_default(&cmd));
}

TEST(GetTests, ExecuteMoveForwardOneWallDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_one_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-test */
TEST(GetTests, FindCommandNodeReturnsMoveForwardOneWallTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-one-wall-test", node->name);
}

TEST(GetTests, ValidateMoveForwardOneWallTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_one_wall_test(&cmd));
}

TEST(GetTests, ValidateMoveForwardOneWallTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_one_wall_test(&cmd));
}

TEST(GetTests, ExecuteMoveForwardOneWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_one_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-current */
TEST(GetTests, FindCommandNodeReturnsMoveForwardOneWallCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-one-wall-current", node->name);
}

TEST(GetTests, ValidateMoveForwardOneWallCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_one_wall_current(&cmd));
}

TEST(GetTests, ValidateMoveForwardOneWallCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-one-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_one_wall_current(&cmd));
}

TEST(GetTests, ExecuteMoveForwardOneWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_one_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-default */
TEST(GetTests, FindCommandNodeReturnsMoveForwardBothWallDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-both-wall-default", node->name);
}

TEST(GetTests, ValidateMoveForwardBothWallDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_both_wall_default(&cmd));
}

TEST(GetTests, ValidateMoveForwardBothWallDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_both_wall_default(&cmd));
}

TEST(GetTests, ExecuteMoveForwardBothWallDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_both_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-test */
TEST(GetTests, FindCommandNodeReturnsMoveForwardBothWallTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-both-wall-test", node->name);
}

TEST(GetTests, ValidateMoveForwardBothWallTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_both_wall_test(&cmd));
}

TEST(GetTests, ValidateMoveForwardBothWallTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_both_wall_test(&cmd));
}

TEST(GetTests, ExecuteMoveForwardBothWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_both_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-current */
TEST(GetTests, FindCommandNodeReturnsMoveForwardBothWallCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("move-forward-both-wall-current", node->name);
}

TEST(GetTests, ValidateMoveForwardBothWallCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_move_forward_both_wall_current(&cmd));
}

TEST(GetTests, ValidateMoveForwardBothWallCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "move-forward-both-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_move_forward_both_wall_current(&cmd));
}

TEST(GetTests, ExecuteMoveForwardBothWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_move_forward_both_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-default */
TEST(GetTests, FindCommandNodeReturnsRotateDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("rotate-default", node->name);
}

TEST(GetTests, ValidateRotateDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_rotate_default(&cmd));
}

TEST(GetTests, ValidateRotateDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_rotate_default(&cmd));
}

TEST(GetTests, ExecuteRotateDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_rotate_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-test */
TEST(GetTests, FindCommandNodeReturnsRotateTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("rotate-test", node->name);
}

TEST(GetTests, ValidateRotateTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_rotate_test(&cmd));
}

TEST(GetTests, ValidateRotateTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_rotate_test(&cmd));
}

TEST(GetTests, ExecuteRotateTestRuns)
{
    struct command cmd{{0}};

    execute_get_rotate_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-current */
TEST(GetTests, FindCommandNodeReturnsRotateCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("rotate-current", node->name);
}

TEST(GetTests, ValidateRotateCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_rotate_current(&cmd));
}

TEST(GetTests, ValidateRotateCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "rotate-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_rotate_current(&cmd));
}

TEST(GetTests, ExecuteRotateCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_rotate_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-default */
TEST(GetTests, FindCommandNodeReturnsFrontWallDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("front-wall-default", node->name);
}

TEST(GetTests, ValidateFrontWallDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_front_wall_default(&cmd));
}

TEST(GetTests, ValidateFrontWallDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_front_wall_default(&cmd));
}

TEST(GetTests, ExecuteFrontWallDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_front_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-test */
TEST(GetTests, FindCommandNodeReturnsFrontWallTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("front-wall-test", node->name);
}

TEST(GetTests, ValidateFrontWallTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_rotate_test(&cmd));
}

TEST(GetTests, ValidateFrontWallTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_rotate_test(&cmd));
}

TEST(GetTests, ExecuteFrontWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_rotate_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-current */
TEST(GetTests, FindCommandNodeReturnsFrontWallCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("front-wall-current", node->name);
}

TEST(GetTests, ValidateFrontWallCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_rotate_current(&cmd));
}

TEST(GetTests, ValidateFrontWallCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "front-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_rotate_current(&cmd));
}

TEST(GetTests, ExecuteFrontWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_rotate_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-default */
TEST(GetTests, FindCommandNodeReturnsSideWallDefaultNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-default";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("side-wall-default", node->name);
}

TEST(GetTests, ValidateSideWallDefaultReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_side_wall_default(&cmd));
}

TEST(GetTests, ValidateSideWallDefaultReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-default";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_side_wall_default(&cmd));
}

TEST(GetTests, ExecuteSideWallDefaultRuns)
{
    struct command cmd{{0}};

    execute_get_side_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-test */
TEST(GetTests, FindCommandNodeReturnsSideWallTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("side-wall-test", node->name);
}

TEST(GetTests, ValidateSideWallTestReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_side_wall_test(&cmd));
}

TEST(GetTests, ValidateSideWallTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_side_wall_test(&cmd));
}

TEST(GetTests, ExecuteSideWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_side_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-current */
TEST(GetTests, FindCommandNodeReturnsSideWallCurrentNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-current";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("side-wall-current", node->name);
}

TEST(GetTests, ValidateSideWallCurrentReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_side_wall_current(&cmd));
}

TEST(GetTests, ValidateSideWallCurrentReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-current";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_side_wall_current(&cmd));
}

TEST(GetTests, ExecuteSideWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_side_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-calculated */
TEST(GetTests, FindCommandNodeReturnsSideWallCalculatedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-calculated";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("side-wall-calculated", node->name);
}

TEST(GetTests, ValidateSideWallCalculatedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get_side_wall_calculated(&cmd));
}

TEST(GetTests, ValidateSideWallCalculatedReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "get";
    cmd.tokens[1] = "side-wall-calculated";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_side_wall_calculated(&cmd));
}

TEST(GetTests, ExecuteSideWallCalculatedRuns)
{
    struct command cmd{{0}};

    execute_get_side_wall_calculated(&cmd);
}
