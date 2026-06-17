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
    fake_root_commands[0].execute = execute_get;
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

struct get_command_test_case {
    const char *name;
    enum validation_result (*validate)(struct command *);
};

static const get_command_test_case get_commands[] = {
    {"solver-default", validate_get_solver_default},
    {"solver-test", validate_get_solver_test},
    {"solver-current", validate_get_solver_current},
    {"mouse-physical-default", validate_get_mouse_physical_default},
    {"mouse-physical-test", validate_get_mouse_physical_test},
    {"mouse-physical-current", validate_get_mouse_physical_current},
    {"mouse-calculated", validate_get_mouse_calculated},
    {"maze-physical-default", validate_get_maze_physical_default},
    {"maze-physical-test", validate_get_maze_physical_test},
    {"maze-physical-current", validate_get_maze_physical_current},
    {"maze-calculated", validate_get_maze_calculated},
    {"navigation", validate_get_navigation},
    {"move-forward-common-default", validate_get_move_forward_common_default},
    {"move-forward-common-test", validate_get_move_forward_common_test},
    {"move-forward-common-current", validate_get_move_forward_common_current},
    {"move-forward-no-wall-default", validate_get_move_forward_no_wall_default},
    {"move-forward-no-wall-test", validate_get_move_forward_no_wall_test},
    {"move-forward-no-wall-current", validate_get_move_forward_no_wall_current},
    {"move-forward-no-wall-calculated", validate_get_move_forward_no_wall_calculated},
    {"move-forward-one-wall-default", validate_get_move_forward_one_wall_default},
    {"move-forward-one-wall-test", validate_get_move_forward_one_wall_test},
    {"move-forward-one-wall-current", validate_get_move_forward_one_wall_current},
    {"move-forward-one-wall-calculated", validate_get_move_forward_one_wall_calculated},
    {"move-forward-both-wall-default", validate_get_move_forward_both_wall_default},
    {"move-forward-both-wall-test", validate_get_move_forward_both_wall_test},
    {"move-forward-both-wall-current", validate_get_move_forward_both_wall_current},
    {"move-forward-both-wall-calculated", validate_get_move_forward_both_wall_calculated},
    {"rotate-default", validate_get_rotate_default},
    {"rotate-test", validate_get_rotate_test},
    {"rotate-current", validate_get_rotate_current},
    {"front-wall-default", validate_get_front_wall_default},
    {"front-wall-test", validate_get_front_wall_test},
    {"front-wall-current", validate_get_front_wall_current},
    {"side-wall-default", validate_get_side_wall_default},
    {"side-wall-test", validate_get_side_wall_test},
    {"side-wall-current", validate_get_side_wall_current},
    {"side-wall-calculated", validate_get_side_wall_calculated}};

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

struct maze_solver_config get_saved_default_maze_solver_config(void)
{
    mock().actualCall("get_saved_default_maze_solver_config");
    struct maze_solver_config cfg{};
    return cfg;
}

struct maze_solver_config get_saved_test_maze_solver_config(void)
{
    mock().actualCall("get_saved_test_maze_solver_config");
    struct maze_solver_config cfg{};
    return cfg;
}

struct maze_solver_config get_maze_solver_config(void)
{
    mock().actualCall("get_maze_solver_config");
    struct maze_solver_config cfg{};
    return cfg;
}

struct mouse_physical_params get_mouse_physical_params(void)
{
    mock().actualCall("get_mouse_physical_params");
    struct mouse_physical_params p{};
    return p;
}

struct mouse_calculated_params get_mouse_calculated_params(void)
{
    mock().actualCall("get_mouse_calculated_params");
    struct mouse_calculated_params p{};
    return p;
}

struct maze_physical_params get_maze_physical_params(void)
{
    mock().actualCall("get_maze_physical_params");
    struct maze_physical_params p{};
    return p;
}

struct maze_calculated_params get_maze_calculated_params(void)
{
    mock().actualCall("get_maze_calculated_params");
    struct maze_calculated_params p{};
    return p;
}

struct navigation_params get_navigation_params(void)
{
    mock().actualCall("get_navigation_params");
    struct navigation_params p{};
    return p;
}

struct mouse_physical_params get_saved_default_mouse_physical_params(void)
{
    mock().actualCall("get_saved_default_mouse_physical_params");
    struct mouse_physical_params p{};
    return p;
}

struct mouse_physical_params get_saved_test_mouse_physical_params(void)
{
    mock().actualCall("get_saved_test_mouse_physical_params");
    struct mouse_physical_params p{};
    return p;
}

struct maze_physical_params get_saved_default_maze_physical_params(void)
{
    mock().actualCall("get_saved_default_maze_physical_params");
    struct maze_physical_params p{};
    return p;
}

struct maze_physical_params get_saved_test_maze_physical_params(void)
{
    mock().actualCall("get_saved_test_maze_physical_params");
    struct maze_physical_params p{};
    return p;
}

struct move_forward_common_config get_saved_default_move_forward_common_config(void)
{
    mock().actualCall("get_saved_default_move_forward_common_config");
    struct move_forward_common_config cfg{};
    return cfg;
}

struct move_forward_common_config get_saved_test_move_forward_common_config(void)
{
    mock().actualCall("get_saved_test_move_forward_common_config");
    struct move_forward_common_config cfg{};
    return cfg;
}

struct move_forward_common_config get_move_forward_common_config(void)
{
    mock().actualCall("get_move_forward_common_config");
    struct move_forward_common_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_no_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_no_wall_config(void)
{
    mock().actualCall("get_saved_test_move_forward_control_no_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_one_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_one_wall_config(void)
{
    mock().actualCall("get_saved_test_move_forward_control_one_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void)
{
    mock().actualCall("get_saved_default_move_forward_control_both_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_saved_test_move_forward_control_both_wall_config(void)
{
    mock().actualCall("get_saved_test_move_forward_control_both_wall_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_no_wall_move_forward_control_config(void)
{
    mock().actualCall("get_no_wall_move_forward_control_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_one_wall_move_forward_control_config(void)
{
    mock().actualCall("get_one_wall_move_forward_control_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_control_config get_both_wall_move_forward_control_config(void)
{
    mock().actualCall("get_both_wall_move_forward_control_config");
    struct move_forward_control_config cfg{};
    return cfg;
}

struct move_forward_calculated_params get_no_wall_move_forward_calculated_params(void)
{
    mock().actualCall("get_no_wall_move_forward_calculated_params");

    struct move_forward_calculated_params p{};
    return p;
}

struct move_forward_calculated_params get_one_wall_move_forward_calculated_params(void)
{
    mock().actualCall("get_one_wall_move_forward_calculated_params");

    struct move_forward_calculated_params p{};
    return p;
}

struct move_forward_calculated_params get_both_wall_move_forward_calculated_params(void)
{
    mock().actualCall("get_both_wall_move_forward_calculated_params");

    struct move_forward_calculated_params p{};
    return p;
}

struct rotate_control_config get_saved_default_rotate_control_config(void)
{
    mock().actualCall("get_saved_default_rotate_control_config");
    struct rotate_control_config cfg{};
    return cfg;
}

struct rotate_control_config get_saved_test_rotate_control_config(void)
{
    mock().actualCall("get_saved_test_rotate_control_config");
    struct rotate_control_config cfg{};
    return cfg;
}

struct rotate_control_config get_rotate_control_config(void)
{
    mock().actualCall("get_rotate_control_config");
    struct rotate_control_config cfg{};
    return cfg;
}

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void)
{
    mock().actualCall("get_saved_default_front_wall_detection_config");
    struct front_wall_detection_config cfg{};
    return cfg;
}

struct front_wall_detection_config get_saved_test_front_wall_detection_config(void)
{
    mock().actualCall("get_saved_test_front_wall_detection_config");
    struct front_wall_detection_config cfg{};
    return cfg;
}

struct front_wall_detection_config get_front_wall_detection_config(void)
{
    mock().actualCall("get_front_wall_detection_config");
    struct front_wall_detection_config cfg{};
    return cfg;
}

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void)
{
    mock().actualCall("get_saved_default_side_wall_detection_config");
    struct side_wall_detection_config cfg{};
    return cfg;
}

struct side_wall_detection_config get_saved_test_side_wall_detection_config(void)
{
    mock().actualCall("get_saved_test_side_wall_detection_config");
    struct side_wall_detection_config cfg{};
    return cfg;
}

struct side_wall_detection_config get_side_wall_detection_config(void)
{
    mock().actualCall("get_side_wall_detection_config");
    struct side_wall_detection_config cfg{};
    return cfg;
}

struct side_wall_calculated_params get_side_wall_calculated_params(void)
{
    mock().actualCall("get_side_wall_calculated_params");
    struct side_wall_calculated_params p{};
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
    POINTERS_EQUAL(validate_get, node->validate);
    POINTERS_EQUAL(execute_get, node->execute);
    POINTERS_EQUAL(get_get_commands(), node->children);
    LONGS_EQUAL(get_get_commands_count(), node->child_count);
}

TEST(GetTests, GetCommandsAreConfiguredCorrectly)
{
    const struct command_node *commands = get_get_commands();

    LONGS_EQUAL(sizeof(get_commands) / sizeof(get_commands[0]), get_get_commands_count());

    for (uint32_t i{0}; i < get_get_commands_count(); i++) {
        STRCMP_EQUAL(get_commands[i].name, commands[i].name);
        POINTERS_EQUAL(get_commands[i].validate, commands[i].validate);
        CHECK(commands[i].execute != nullptr);
    }
}

TEST(GetTests, FindCommandNodeReturnsExpectedNode)
{
    for (auto const &test : get_commands) {
        check_command_lookup(test.name, 2);
    }
}

TEST(GetTests, ValidateCommandsReturnSuccess)
{
    for (auto const &test : get_commands) {
        check_validation_success(test.name, 2, test.validate);
    }
}

TEST(GetTests, ValidateCommandsReturnTooManyParameters)
{
    for (auto const &test : get_commands) {
        check_validation_too_many_params(test.name, 3, test.validate);
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

TEST(GetTests, ValidateGetWithoutParametersReturnsSuccess)
{
    struct command cmd{make_get_command("", 1)};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_get(&cmd));
}

TEST(GetTests, ValidateGetReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get(&cmd));
}

TEST(GetTests, ExecuteGetRuns)
{
    struct command cmd{{0}};

    execute_get(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
TEST(GetTests, ExecuteGetSolverDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_maze_solver_config");
    execute_get_solver_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-test */
TEST(GetTests, ExecuteGetSolverTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_maze_solver_config");
    execute_get_solver_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get solver-current */
TEST(GetTests, ExecuteGetSolverCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_maze_solver_config");
    execute_get_solver_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-default */
TEST(GetTests, ExecuteGetMousePhysicalDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_mouse_physical_params");
    execute_get_mouse_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-test */
TEST(GetTests, ExecuteGetMousePhysicalDefaultTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_mouse_physical_params");
    execute_get_mouse_physical_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-physical-current */
TEST(GetTests, ExecuteGetMousePhysicalDefaultCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_mouse_physical_params");
    execute_get_mouse_physical_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get mouse-calculated */
TEST(GetTests, ExecuteGetMouseCalculatedRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_mouse_calculated_params");
    execute_get_mouse_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-default */
TEST(GetTests, ExecuteGetMazePhysicalDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_maze_physical_params");
    execute_get_maze_physical_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-test */
TEST(GetTests, ExecuteGetMazePhysicalTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_maze_physical_params");
    execute_get_maze_physical_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-physical-current */
TEST(GetTests, ExecuteGetMazePhysicalCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_maze_physical_params");
    execute_get_maze_physical_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get maze-calculated */
TEST(GetTests, ExecuteGetMazeCalculatedRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_maze_calculated_params");
    execute_get_maze_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get navigation */
TEST(GetTests, ExecuteGetNavigationRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_navigation_params");
    execute_get_navigation(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-common-default */
TEST(GetTests, ExecuteMoveForwardCommonDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_move_forward_common_config");
    execute_get_move_forward_common_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-common-test */
TEST(GetTests, ExecuteMoveForwardCommonTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_move_forward_common_config");
    execute_get_move_forward_common_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-common-current */
TEST(GetTests, ExecuteMoveForwardCommonCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_move_forward_common_config");
    execute_get_move_forward_common_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-default */
TEST(GetTests, ExecuteMoveForwardNoWallDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_move_forward_control_no_wall_config");
    execute_get_move_forward_no_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-test */
TEST(GetTests, ExecuteMoveForwardNoWallTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_move_forward_control_no_wall_config");
    execute_get_move_forward_no_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-current */
TEST(GetTests, ExecuteMoveForwardNoWallCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_no_wall_move_forward_control_config");
    execute_get_move_forward_no_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-no-wall-calculated */
TEST(GetTests, ExecuteMoveForwardNoWallCalculatedRuns)
{
    struct command cmd{{0}};

    mock().expectOneCall("get_no_wall_move_forward_calculated_params");

    execute_get_move_forward_no_wall_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-default */
TEST(GetTests, ExecuteMoveForwardOneWallDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_move_forward_control_one_wall_config");
    execute_get_move_forward_one_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-test */
TEST(GetTests, ExecuteMoveForwardOneWallTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_move_forward_control_one_wall_config");
    execute_get_move_forward_one_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-current */
TEST(GetTests, ExecuteMoveForwardOneWallCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_one_wall_move_forward_control_config");
    execute_get_move_forward_one_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-one-wall-calculated */
TEST(GetTests, ExecuteMoveForwardOneWallCalculatedRuns)
{
    struct command cmd{{0}};

    mock().expectOneCall("get_one_wall_move_forward_calculated_params");

    execute_get_move_forward_one_wall_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-default */
TEST(GetTests, ExecuteMoveForwardBothWallDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_move_forward_control_both_wall_config");
    execute_get_move_forward_both_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-test */
TEST(GetTests, ExecuteMoveForwardBothWallTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_move_forward_control_both_wall_config");
    execute_get_move_forward_both_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-current */
TEST(GetTests, ExecuteMoveForwardBothWallCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_both_wall_move_forward_control_config");
    execute_get_move_forward_both_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get move-forward-both-wall-calculated */
TEST(GetTests, ExecuteMoveForwardBothWallCalculatedRuns)
{
    struct command cmd{{0}};

    mock().expectOneCall("get_both_wall_move_forward_calculated_params");

    execute_get_move_forward_both_wall_calculated(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-default */
TEST(GetTests, ExecuteRotateDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_rotate_control_config");
    execute_get_rotate_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-test */
TEST(GetTests, ExecuteRotateTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_rotate_control_config");
    execute_get_rotate_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get rotate-current */
TEST(GetTests, ExecuteRotateCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_rotate_control_config");
    execute_get_rotate_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-default */
TEST(GetTests, ExecuteFrontWallDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_front_wall_detection_config");
    execute_get_front_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-test */
TEST(GetTests, ExecuteFrontWallTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_front_wall_detection_config");
    execute_get_front_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get front-wall-current */
TEST(GetTests, ExecuteFrontWallCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_front_wall_detection_config");
    execute_get_front_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-default */
TEST(GetTests, ExecuteSideWallDefaultRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_default_side_wall_detection_config");
    execute_get_side_wall_default(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-test */
TEST(GetTests, ExecuteSideWallTestRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_saved_test_side_wall_detection_config");
    execute_get_side_wall_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-current */
TEST(GetTests, ExecuteSideWallCurrentRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_side_wall_detection_config");
    execute_get_side_wall_current(&cmd);
}

/*----------------------------------------------------------------------------*/
/* get side-wall-calculated */
TEST(GetTests, ExecuteSideWallCalculatedRuns)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_side_wall_calculated_params");
    execute_get_side_wall_calculated(&cmd);
}
