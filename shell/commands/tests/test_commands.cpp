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
TEST(CommandsTests, GetCommandTableSizeReturnsExpectedValue)
{
    LONGS_EQUAL(3u, get_command_table_size());
}

TEST(CommandsTests, GetCommandTableEntryAtIndexReturnsHelpCommand)
{
    struct command_table_entry entry{get_command_table_entry_at_index(0u)};

    STRCMP_EQUAL("help", entry.name);
    STRCMP_EQUAL("Display available commands", entry.help);

    CHECK(entry.validate != nullptr);
    CHECK(entry.execute != nullptr);
}

TEST(CommandsTests, GetCommandTableEntryAtIndexReturnsEmptyEntryWhenOutOfRange)
{
    struct command_table_entry entry{get_command_table_entry_at_index(999u)};

    POINTERS_EQUAL(nullptr, entry.name);
    POINTERS_EQUAL(nullptr, entry.help);
    POINTERS_EQUAL(nullptr, entry.validate);
    POINTERS_EQUAL(nullptr, entry.execute);
}

TEST(CommandsTests, ValidateHelpReturnsSuccess)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "help");

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_help(&cmd));
}

TEST(CommandsTests, ValidateHelpReturnsTooManyParameters)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "help");
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_help(&cmd));
}

TEST(CommandsTests, ValidateHelpReturnsNotMatched)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "motor");

    LONGS_EQUAL(COMMAND_VALIDATION_NOT_MATCHED, validate_help(&cmd));
}

TEST(CommandsTests, ValidateClearReturnsSuccess)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "clear");

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_clear(&cmd));
}

TEST(CommandsTests, ValidateClearReturnsTooManyParameters)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "clear");
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_clear(&cmd));
}

TEST(CommandsTests, ValidateClearReturnsNotMatched)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "invalid");

    LONGS_EQUAL(COMMAND_VALIDATION_NOT_MATCHED, validate_clear(&cmd));
}

TEST(CommandsTests, ValidateHardwareFaultsReturnsSuccess)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "faults");

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_hardware_faults(&cmd));
}

TEST(CommandsTests, ValidateHardwareFaultsReturnsTooManyParameters)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "faults");
    cmd.parameter_count = 1;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_hardware_faults(&cmd));
}

TEST(CommandsTests, ValidateHardwareFaultsReturnsNotMatched)
{
    struct command cmd{{0}};

    strcpy(cmd.command, "invalid");

    LONGS_EQUAL(COMMAND_VALIDATION_NOT_MATCHED, validate_hardware_faults(&cmd));
}

TEST(CommandsTests, ExecuteHardwareFaultsCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("print_hardware_state");

    execute_hardware_faults(&cmd);
}
