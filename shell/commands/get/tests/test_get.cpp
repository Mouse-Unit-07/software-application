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

struct command make_get_command(char const *subcommand, uint32_t token_count)
{
    struct command cmd{{0}};

    cmd.token_count = token_count;
    cmd.tokens[0] = "get";
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

TEST(GetTests, GetCommandsAreConfiguredCorrectly)
{
    static const char *expected_names[] =
    {
        "solver-default",
        "solver-test",
        "solver-current",
        "mouse-physical-default",
        "mouse-physical-test",
        "mouse-physical-current",
        "mouse-calculated",
        "maze-physical-default",
        "maze-physical-test",
        "maze-physical-current",
        "maze-calculated",
        "navigation",
        "move-forward-no-wall-default",
        "move-forward-no-wall-test",
        "move-forward-no-wall-current",
        "move-forward-one-wall-default",
        "move-forward-one-wall-test",
        "move-forward-one-wall-current",
        "move-forward-both-wall-default",
        "move-forward-both-wall-test",
        "move-forward-both-wall-current",
        "rotate-default",
        "rotate-test",
        "rotate-current",
        "front-wall-default",
        "front-wall-test",
        "front-wall-current",
        "side-wall-default",
        "side-wall-test",
        "side-wall-current",
        "side-wall-calculated",
    };

    const struct command_node *commands = get_get_commands();

    LONGS_EQUAL(sizeof(expected_names) / sizeof(expected_names[0]), get_get_commands_count());

    for (uint32_t i{0}; i < get_get_commands_count(); i++) {
        STRCMP_EQUAL(expected_names[i], commands[i].name);
        CHECK(commands[i].validate != nullptr);
        CHECK(commands[i].execute != nullptr);
    }
}

/*----------------------------------------------------------------------------*/
/* get */
TEST(GetTests, FindCommandNodeReturnsGetNode)
{
    struct command cmd{make_get_command("", 1)};

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("get", node->name);
}

TEST(GetTests, ValidateGetReturnsSuccess)
{
    check_validation_success("", 2, validate_get);
}

TEST(GetTests, ValidateGetReturnsTooFewParameters)
{
    struct command cmd{make_get_command("", 1)};

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_get(&cmd));
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
TEST(GetTests, FindCommandNodeReturnsGetSolverDefaultNode)
{
    check_command_lookup("solver-default", 2);
}

TEST(GetTests, ValidateGetSolverDefaultReturnsSuccess)
{
    check_validation_success("solver-default", 2, validate_get_solver_default);
}

TEST(GetTests, ValidateGetSolverDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("solver-default", 3, validate_get_solver_default);
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
    check_command_lookup("solver-test", 2);
}

TEST(GetTests, ValidateGetSolverTestReturnsSuccess)
{
    check_validation_success("solver-test", 2, validate_get_solver_test);
}

TEST(GetTests, ValidateGetSolverTestReturnsTooManyParameters)
{
    check_validation_too_many_params("solver-test", 3, validate_get_solver_test);
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
    check_command_lookup("solver-current", 2);
}

TEST(GetTests, ValidateGetSolverCurrentReturnsSuccess)
{
    check_validation_success("solver-current", 2, validate_get_solver_current);
}

TEST(GetTests, ValidateGetSolverCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("solver-current", 3, validate_get_solver_current);
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
    check_command_lookup("mouse-physical-default", 2);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultReturnsSuccess)
{
    check_validation_success("mouse-physical-default", 2, validate_get_mouse_physical_default);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("mouse-physical-default", 3, validate_get_mouse_physical_default);
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
    check_command_lookup("mouse-physical-test", 2);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultTestReturnsSuccess)
{
    check_validation_success("mouse-physical-test", 2, validate_get_mouse_physical_test);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultTestReturnsTooManyParameters)
{
    check_validation_too_many_params("mouse-physical-test", 3, validate_get_mouse_physical_test);
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
    check_command_lookup("mouse-physical-current", 2);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultCurrentReturnsSuccess)
{
    check_validation_success("mouse-physical-current", 2, validate_get_mouse_physical_current);
}

TEST(GetTests, ValidateGetMousePhysicalDefaultCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("mouse-physical-current", 3, validate_get_mouse_physical_current);
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
    check_command_lookup("mouse-calculated", 2);
}

TEST(GetTests, ValidateGetMouseCalculatedReturnsSuccess)
{
    check_validation_success("mouse-calculated", 2, validate_get_mouse_calculated);
}

TEST(GetTests, ValidateGetMouseCalculatedReturnsTooManyParameters)
{
    check_validation_too_many_params("mouse-calculated", 3, validate_get_mouse_calculated);
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
    check_command_lookup("maze-physical-default", 2);
}

TEST(GetTests, ValidateGetMazePhysicalDefaultReturnsSuccess)
{
    check_validation_success("maze-physical-default", 2, validate_get_maze_physical_default);
}

TEST(GetTests, ValidateGetMazePhysicalDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-physical-default", 3, validate_get_maze_physical_default);
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
    check_command_lookup("maze-physical-test", 2);
}

TEST(GetTests, ValidateGetMazePhysicalTestReturnsSuccess)
{
    check_validation_success("maze-physical-test", 2, validate_get_maze_physical_test);
}

TEST(GetTests, ValidateGetMazePhysicalTestReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-physical-test", 3, validate_get_maze_physical_test);
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
    check_command_lookup("maze-physical-current", 2);
}

TEST(GetTests, ValidateGetMazePhysicalCurrentReturnsSuccess)
{
    check_validation_success("maze-physical-current", 2, validate_get_maze_physical_current);
}

TEST(GetTests, ValidateGetMazePhysicalCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-physical-current", 3, validate_get_maze_physical_current);
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
    check_command_lookup("maze-calculated", 2);
}

TEST(GetTests, ValidateGetMazeCalculatedReturnsSuccess)
{
    check_validation_success("maze-calculated", 2, validate_get_maze_calculated);
}

TEST(GetTests, ValidateGetMazeCalculatedReturnsTooManyParameters)
{
    check_validation_too_many_params("maze-calculated", 3, validate_get_maze_calculated);
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
    check_command_lookup("navigation", 2);
}

TEST(GetTests, ValidateGetNavigationReturnsSuccess)
{
    check_validation_success("navigation", 2, validate_get_navigation);
}

TEST(GetTests, ValidateGetNavigationReturnsTooManyParameters)
{
    check_validation_too_many_params("navigation", 3, validate_get_navigation);
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
    check_command_lookup("move-forward-no-wall-default", 2);
}

TEST(GetTests, ValidateMoveForwardNoWallDefaultReturnsSuccess)
{
    check_validation_success("move-forward-no-wall-default", 2, validate_get_move_forward_no_wall_default);
}

TEST(GetTests, ValidateMoveForwardNoWallDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-no-wall-default", 3, validate_get_move_forward_no_wall_default);
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
    check_command_lookup("move-forward-no-wall-test", 2);
}

TEST(GetTests, ValidateMoveForwardNoWallTestReturnsSuccess)
{
    check_validation_success("move-forward-no-wall-test", 2, validate_get_move_forward_no_wall_test);
}

TEST(GetTests, ValidateMoveForwardNoWallTestReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-no-wall-test", 3, validate_get_move_forward_no_wall_test);
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
    check_command_lookup("move-forward-no-wall-current", 2);
}

TEST(GetTests, ValidateMoveForwardNoWallCurrentReturnsSuccess)
{
    check_validation_success("move-forward-no-wall-current", 2, validate_get_move_forward_no_wall_current);
}

TEST(GetTests, ValidateMoveForwardNoWallCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-no-wall-current", 3, validate_get_move_forward_no_wall_current);
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
    check_command_lookup("move-forward-one-wall-default", 2);
}

TEST(GetTests, ValidateMoveForwardOneWallDefaultReturnsSuccess)
{
    check_validation_success("move-forward-one-wall-default", 2, validate_get_move_forward_one_wall_default);
}

TEST(GetTests, ValidateMoveForwardOneWallDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-one-wall-default", 3, validate_get_move_forward_one_wall_default);
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
    check_command_lookup("move-forward-one-wall-test", 2);
}

TEST(GetTests, ValidateMoveForwardOneWallTestReturnsSuccess)
{
    check_validation_success("move-forward-one-wall-test", 2, validate_get_move_forward_one_wall_test);
}

TEST(GetTests, ValidateMoveForwardOneWallTestReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-one-wall-test", 3, validate_get_move_forward_one_wall_test);
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
    check_command_lookup("move-forward-one-wall-current", 2);
}

TEST(GetTests, ValidateMoveForwardOneWallCurrentReturnsSuccess)
{
    check_validation_success("move-forward-one-wall-current", 2, validate_get_move_forward_one_wall_current);
}

TEST(GetTests, ValidateMoveForwardOneWallCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-one-wall-current", 3, validate_get_move_forward_one_wall_current);
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
    check_command_lookup("move-forward-both-wall-default", 2);
}

TEST(GetTests, ValidateMoveForwardBothWallDefaultReturnsSuccess)
{
    check_validation_success("move-forward-both-wall-default", 2, validate_get_move_forward_both_wall_default);
}

TEST(GetTests, ValidateMoveForwardBothWallDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-both-wall-default", 3, validate_get_move_forward_both_wall_default);
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
    check_command_lookup("move-forward-both-wall-test", 2);
}

TEST(GetTests, ValidateMoveForwardBothWallTestReturnsSuccess)
{
    check_validation_success("move-forward-both-wall-test", 2, validate_get_move_forward_both_wall_test);
}

TEST(GetTests, ValidateMoveForwardBothWallTestReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-both-wall-test", 3, validate_get_move_forward_both_wall_test);
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
    check_command_lookup("move-forward-both-wall-current", 2);
}

TEST(GetTests, ValidateMoveForwardBothWallCurrentReturnsSuccess)
{
    check_validation_success("move-forward-both-wall-current", 2, validate_get_move_forward_both_wall_current);
}

TEST(GetTests, ValidateMoveForwardBothWallCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("move-forward-both-wall-current", 3, validate_get_move_forward_both_wall_current);
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
    check_command_lookup("rotate-default", 2);
}

TEST(GetTests, ValidateRotateDefaultReturnsSuccess)
{
    check_validation_success("rotate-default", 2, validate_get_rotate_default);
}

TEST(GetTests, ValidateRotateDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("rotate-default", 3, validate_get_rotate_default);
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
    check_command_lookup("rotate-test", 2);
}

TEST(GetTests, ValidateRotateTestReturnsSuccess)
{
    check_validation_success("rotate-test", 2, validate_get_rotate_test);
}

TEST(GetTests, ValidateRotateTestReturnsTooManyParameters)
{
    check_validation_too_many_params("rotate-test", 3, validate_get_rotate_test);
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
    check_command_lookup("rotate-current", 2);
}

TEST(GetTests, ValidateRotateCurrentReturnsSuccess)
{
    check_validation_success("rotate-current", 2, validate_get_rotate_current);
}

TEST(GetTests, ValidateRotateCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("rotate-current", 3, validate_get_rotate_current);
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
    check_command_lookup("front-wall-default", 2);
}

TEST(GetTests, ValidateFrontWallDefaultReturnsSuccess)
{
    check_validation_success("front-wall-default", 2, validate_get_front_wall_default);
}

TEST(GetTests, ValidateFrontWallDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("front-wall-default", 3, validate_get_front_wall_default);
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
    check_command_lookup("front-wall-test", 2);
}

TEST(GetTests, ValidateFrontWallTestReturnsSuccess)
{
    check_validation_success("front-wall-test", 2, validate_get_front_wall_test);
}

TEST(GetTests, ValidateFrontWallTestReturnsTooManyParameters)
{
    check_validation_too_many_params("front-wall-test", 3, validate_get_front_wall_test);
}

TEST(GetTests, ExecuteFrontWallTestRuns)
{
    struct command cmd{{0}};

    execute_get_front_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-current */
TEST(GetTests, FindCommandNodeReturnsFrontWallCurrentNode)
{
    check_command_lookup("front-wall-current", 2);
}

TEST(GetTests, ValidateFrontWallCurrentReturnsSuccess)
{
    check_validation_success("front-wall-current", 2, validate_get_front_wall_current);
}

TEST(GetTests, ValidateFrontWallCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("front-wall-current", 3, validate_get_front_wall_current);
}

TEST(GetTests, ExecuteFrontWallCurrentRuns)
{
    struct command cmd{{0}};

    execute_get_front_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-default */
TEST(GetTests, FindCommandNodeReturnsSideWallDefaultNode)
{
    check_command_lookup("side-wall-default", 2);
}

TEST(GetTests, ValidateSideWallDefaultReturnsSuccess)
{
    check_validation_success("side-wall-default", 2, validate_get_side_wall_default);
}

TEST(GetTests, ValidateSideWallDefaultReturnsTooManyParameters)
{
    check_validation_too_many_params("side-wall-default", 3, validate_get_side_wall_default);
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
    check_command_lookup("side-wall-test", 2);
}

TEST(GetTests, ValidateSideWallTestReturnsSuccess)
{
    check_validation_success("side-wall-test", 2, validate_get_side_wall_test);
}

TEST(GetTests, ValidateSideWallTestReturnsTooManyParameters)
{
    check_validation_too_many_params("side-wall-test", 3, validate_get_side_wall_test);
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
    check_command_lookup("side-wall-current", 2);
}

TEST(GetTests, ValidateSideWallCurrentReturnsSuccess)
{
    check_validation_success("side-wall-current", 2, validate_get_side_wall_current);
}

TEST(GetTests, ValidateSideWallCurrentReturnsTooManyParameters)
{
    check_validation_too_many_params("side-wall-current", 3, validate_get_side_wall_current);
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
    check_command_lookup("side-wall-calculated", 2);
}

TEST(GetTests, ValidateSideWallCalculatedReturnsSuccess)
{
    check_validation_success("side-wall-calculated", 2, validate_get_side_wall_calculated);
}

TEST(GetTests, ValidateSideWallCalculatedReturnsTooManyParameters)
{
    check_validation_too_many_params("side-wall-calculated", 3, validate_get_side_wall_calculated);
}

TEST(GetTests, ExecuteSideWallCalculatedRuns)
{
    struct command cmd{{0}};

    execute_get_side_wall_calculated(&cmd);
}
