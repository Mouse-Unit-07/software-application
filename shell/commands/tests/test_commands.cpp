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
    LONGS_EQUAL(4u, get_command_tree_root_count());
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

    strcpy(cmd.command, "invalid");

    POINTERS_EQUAL(nullptr, find_command_node(&cmd));
}

TEST(CommandsTests, FindCommandNodeReturnsNullForEmptyCommand)
{
    struct command cmd{{0}};

    POINTERS_EQUAL(nullptr, find_command_node(&cmd));
}

/*----------------------------------------------------------------------------*/
/* help */
TEST(CommandsTests, FindHelpCommandReturnsNode)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "help");

    struct command_node const *node = find_command_node(&cmd);

    CHECK(node != nullptr);

    STRCMP_EQUAL("help", node->name);

    POINTERS_EQUAL((void *)validate_help, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_help, (void *)node->execute);
}

TEST(CommandsTests, ValidateHelpReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_help(&cmd));
}

TEST(CommandsTests, ValidateHelpReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_help(&cmd));
}

/*----------------------------------------------------------------------------*/
/* clear */
TEST(CommandsTests, FindClearCommandReturnsNode)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "clear");

    struct command_node const *node = find_command_node(&cmd);

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
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_clear(&cmd));
}

/*----------------------------------------------------------------------------*/
/* faults */
TEST(CommandsTests, FindFaultsCommandReturnsNode)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "faults");

    struct command_node const *node = find_command_node(&cmd);

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
    cmd.parameter_count = 1;

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

    strcpy(cmd.command, "time");

    struct command_node const *node = find_command_node(&cmd);

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
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_get_time(&cmd));
}

TEST(CommandsTests, ExecuteTimeCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_current_global_time_sec")
        .andReturnValue(0u);

    execute_get_time(&cmd);
}
