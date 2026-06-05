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
    LONGS_EQUAL(5u, get_command_tree_root_count());
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
