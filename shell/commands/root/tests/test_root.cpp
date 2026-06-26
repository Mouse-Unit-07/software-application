/*================================ FILE INFO =================================*/
/* Filename           : test_root.cpp                                         */
/*                                                                            */
/* Test implementation for root.c                                             */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdint.h>
#include "command.h"
#include "root.h"

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
    return mock().actualCall("get_current_global_time_sec").returnUnsignedIntValue();
}

struct command_node const *get_get_node(void)
{
    static struct command_node node =
    {
        .name = "get",
        .help = "mock"
    };

    return &node;
}

struct command_node const *get_set_node(void)
{
    static struct command_node node =
    {
        .name = "set",
        .help = "mock"
    };

    return &node;
}

struct command_node const *get_solve_node(void)
{
    static struct command_node node =
    {
        .name = "solve",
        .help = "mock"
    };

    return &node;
}

struct command_node const *get_test_node(void)
{
    static struct command_node node =
    {
        .name = "test",
        .help = "mock"
    };

    return &node;
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(RootTests)
{
    void setup() override
    {
        init_root();
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
        deinit_root();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(RootTests, InitRootAddsCommandModules)
{
    init_root();

    LONGS_EQUAL(8u, get_root_commands_count());
}

TEST(RootTests, InitRootCanBeCalledMultipleTimes)
{
    init_root();
    init_root();
    init_root();

    LONGS_EQUAL(8u, get_root_commands_count());
}

TEST(RootTests, DeinitRootRestoresDefaultCount)
{
    init_root();
    deinit_root();

    LONGS_EQUAL(4u, get_root_commands_count());
}

TEST(RootTests, DeinitRootCanBeCalledMultipleTimes)
{
    deinit_root();
    deinit_root();

    LONGS_EQUAL(4u, get_root_commands_count());
}

TEST(RootTests, GetRootCommandsReturnsValidPointer)
{
    CHECK(get_root_commands() != nullptr);
}

TEST(RootTests, GetRootCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(8u, get_root_commands_count());
}

TEST(RootTests, GetCommandTreeRootContainsHelpNode)
{
    struct command_node const *root = get_root_commands();

    STRCMP_EQUAL("help", root[0].name);

    STRCMP_EQUAL("Display available commands", root[0].help);

    CHECK(root[0].validate != nullptr);
    CHECK(root[0].execute != nullptr);
}

TEST(RootTests, FindCommandNodeReturnsNullForUnknownCommand)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "invalid";

    struct command_match match{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count())};

    POINTERS_EQUAL(nullptr, match.node);
}

TEST(RootTests, FindCommandNodeReturnsNullForEmptyCommand)
{
    struct command cmd{{0}};

    struct command_match match{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count())};

    POINTERS_EQUAL(nullptr, match.node);
}

TEST(RootTests, FindCommandNodeInRootReturnsHelpNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_match match = find_command_node_in_root(&cmd);

    CHECK(match.node != nullptr);
    STRCMP_EQUAL("help", match.node->name);
}

TEST(RootTests, FindCommandNodeInRootReturnsDepthOne)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_match match = find_command_node_in_root(&cmd);

    LONGS_EQUAL(1u, match.depth);
}

TEST(RootTests, RootCommandsAreAddedInExpectedOrder)
{
    struct command_node const *root = get_root_commands();

    STRCMP_EQUAL("help", root[0].name);
    STRCMP_EQUAL("clear", root[1].name);
    STRCMP_EQUAL("faults", root[2].name);
    STRCMP_EQUAL("time", root[3].name);
    STRCMP_EQUAL("get", root[4].name);
    STRCMP_EQUAL("set", root[5].name);
    STRCMP_EQUAL("solve", root[6].name);
    STRCMP_EQUAL("test", root[7].name);
}

/*----------------------------------------------------------------------------*/
/* help */
TEST(RootTests, FindHelpCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_node const *node{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count()).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("help", node->name);

    POINTERS_EQUAL((void *)validate_help, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_help, (void *)node->execute);
}

TEST(RootTests, HelpCommandMatchDepthIsOne)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "help";

    struct command_match match{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count())};

    LONGS_EQUAL(1u, match.depth);
}

TEST(RootTests, ValidateHelpReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_help(&cmd));
}

TEST(RootTests, ValidateHelpReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_help(&cmd));
}

TEST(RootTests, ExecuteHelpRunsWithoutCrash)
{
    struct command cmd{{0}};

    execute_help(&cmd);
}

/*----------------------------------------------------------------------------*/
/* clear */
TEST(RootTests, FindClearCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "clear";

    struct command_node const *node{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count()).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("clear", node->name);

    POINTERS_EQUAL((void *)validate_clear, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_clear, (void *)node->execute);
}

TEST(RootTests, ValidateClearReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_clear(&cmd));
}

TEST(RootTests, ValidateClearReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_clear(&cmd));
}

TEST(RootTests, ExecuteClearRunsWithoutCrash)
{
    struct command cmd{{0}};

    execute_clear(&cmd);
}

/*----------------------------------------------------------------------------*/
/* faults */
TEST(RootTests, FindFaultsCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "faults";

    struct command_node const *node{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count()).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("faults", node->name);

    POINTERS_EQUAL((void *)validate_hardware_faults, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_hardware_faults, (void *)node->execute);
}

TEST(RootTests, ValidateHardwareFaultsReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_hardware_faults(&cmd));
}

TEST(RootTests, ValidateHardwareFaultsReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_hardware_faults(&cmd));
}

TEST(RootTests, ExecuteHardwareFaultsCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("print_hardware_state");

    execute_hardware_faults(&cmd);
}

/*----------------------------------------------------------------------------*/
/* time */
TEST(RootTests, FindTimeCommandReturnsNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "time";

    struct command_node const *node{
            find_command_node(&cmd, get_root_commands(), get_root_commands_count()).node};

    CHECK(node != nullptr);

    STRCMP_EQUAL("time", node->name);

    POINTERS_EQUAL((void *)validate_time, (void *)node->validate);
    POINTERS_EQUAL((void *)execute_time, (void *)node->execute);
}

TEST(RootTests, ValidateTimeReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_time(&cmd));
}

TEST(RootTests, ValidateTimeReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_time(&cmd));
}

TEST(RootTests, ExecuteTimeCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("get_current_global_time_sec").andReturnValue(0u);

    execute_time(&cmd);
}
