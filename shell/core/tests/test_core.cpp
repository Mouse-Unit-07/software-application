/*================================ FILE INFO =================================*/
/* Filename           : test_core.cpp                                         */
/*                                                                            */
/* Test implementation for core.c                                             */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdbool.h>
#include <stdint.h>
#include "command.h"
#include "core.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
void feedCharacters(char const *str)
{
    size_t len = strlen(str);
    bool terminated = false;

    for (size_t i = 0u; i < len; i++) {
        mock().expectOneCall("is_cli_buffer_empty")
              .andReturnValue(false);

        mock().expectOneCall("pop_cli_buffer")
              .andReturnValue(str[i]);

        if ((str[i] == '\n') || (str[i] == '\r')) {
            mock().expectOneCall("clear_cli_buffer");
            terminated = true;
            break;
        }
    }

    if (!terminated) {
        mock().expectOneCall("is_cli_buffer_empty")
              .andReturnValue(true);
    }
}

void init_shell_for_test(void)
{
    mock().disable();
    init_shell();
    mock().enable();
}

void deinit_shell_for_test(void)
{
    mock().disable();
    deinit_shell();
    mock().enable();
}

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

bool is_cli_buffer_empty(void)
{
    return mock()
        .actualCall("is_cli_buffer_empty")
        .returnBoolValue();
}

char pop_cli_buffer(void)
{
    return (char)mock()
        .actualCall("pop_cli_buffer")
        .returnIntValue();
}

void clear_cli_buffer(void)
{
    mock().actualCall("clear_cli_buffer");
}

void init_root(void)
{
    mock().actualCall("init_root");
}

void deinit_root(void)
{
    mock().actualCall("deinit_root");
}

struct command_match find_command_node_in_root(struct command *cmd)
{
    mock().actualCall("find_command_node_in_root")
        .withPointerParameter("cmd", cmd);

    struct command_match match{};
    return match;
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(ShellTests)
{
    void setup() override
    {
        init_shell_for_test();
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
        deinit_shell_for_test();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(ShellTests, InitShellResetsStateAndCallsFunctions)
{
    strcpy(get_shell_buffer(), "abc");
    mock().expectOneCall("init_root");

    init_shell();

    STRCMP_EQUAL("", get_shell_buffer());
    LONGS_EQUAL(0u, get_shell_buffer_size());
    CHECK_FALSE(get_ready_for_parsing());
}

TEST(ShellTests, DeinitShellResetsStateAndCallsFunctions)
{
    strcpy(get_shell_buffer(), "abc");
    mock().expectOneCall("deinit_root");

    deinit_shell();

    STRCMP_EQUAL("", get_shell_buffer());
    LONGS_EQUAL(0u, get_shell_buffer_size());
    CHECK_FALSE(get_ready_for_parsing());
}

TEST(ShellTests, LoadCliBufferCopiesSingleCharacter)
{
    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(false);

    mock().expectOneCall("pop_cli_buffer")
          .andReturnValue('a');

    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(true);

    load_cli_buffer_contents();

    STRCMP_EQUAL("a", get_shell_buffer());
    LONGS_EQUAL(1u, get_shell_buffer_size());
}

TEST(ShellTests, LoadCliBufferCopiesMultipleCharacters)
{
    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(false);

    mock().expectOneCall("pop_cli_buffer")
          .andReturnValue('a');

    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(false);

    mock().expectOneCall("pop_cli_buffer")
          .andReturnValue('b');

    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(true);

    load_cli_buffer_contents();

    STRCMP_EQUAL("ab", get_shell_buffer());
    LONGS_EQUAL(2u, get_shell_buffer_size());
}

TEST(ShellTests, BackspaceDeletesPreviousCharacter)
{
    feedCharacters("abc\b");

    load_cli_buffer_contents();

    STRCMP_EQUAL("ab", get_shell_buffer());
    LONGS_EQUAL(2u, get_shell_buffer_size());
}

TEST(ShellTests, CarriageReturnSetsReadyForParsing)
{
    feedCharacters("help\r");

    load_cli_buffer_contents();

    CHECK_TRUE(get_ready_for_parsing());
}

TEST(ShellTests, NewlineSetsReadyForParsing)
{
    feedCharacters("help\n");

    load_cli_buffer_contents();

    CHECK_TRUE(get_ready_for_parsing());
}

TEST(ShellTests, EnterClearsCliBuffer)
{
    mock().expectOneCall("is_cli_buffer_empty")
          .andReturnValue(false);

    mock().expectOneCall("pop_cli_buffer")
          .andReturnValue('\r');

    mock().expectOneCall("clear_cli_buffer");

    load_cli_buffer_contents();
}

TEST(ShellTests, ParseEmptyBufferReturnsEmptyCommand)
{
    struct command cmd{parse_cli_buffer_contents()};

    STRCMP_EQUAL(nullptr, cmd.tokens[0]);
    LONGS_EQUAL(0u, cmd.token_count);
}

TEST(ShellTests, ParseCommandOnly)
{
    strcpy(get_shell_buffer(), "help");

    struct command cmd{parse_cli_buffer_contents()};

    STRCMP_EQUAL("help", cmd.tokens[0]);
    LONGS_EQUAL(1u, cmd.token_count);
}

TEST(ShellTests, ParseCommandWithOneParameter)
{
    strcpy(get_shell_buffer(), "motor 100");

    struct command cmd{parse_cli_buffer_contents()};

    STRCMP_EQUAL("motor", cmd.tokens[0]);
    LONGS_EQUAL(2u, cmd.token_count);
    STRCMP_EQUAL("100", cmd.tokens[1]);
}

TEST(ShellTests, ParseCommandWithMultipleParameters)
{
    strcpy(get_shell_buffer(), "motor 100 200 300");

    struct command cmd{parse_cli_buffer_contents()};

    STRCMP_EQUAL("motor", cmd.tokens[0]);
    LONGS_EQUAL(4u, cmd.token_count);
    STRCMP_EQUAL("100", cmd.tokens[1]);
    STRCMP_EQUAL("200", cmd.tokens[2]);
    STRCMP_EQUAL("300", cmd.tokens[3]);
}

TEST(ShellTests, ParseMultipleSpaces)
{
    strcpy(get_shell_buffer(), "motor    100     200");

    struct command cmd{parse_cli_buffer_contents()};

    STRCMP_EQUAL("motor", cmd.tokens[0]);
    LONGS_EQUAL(3u, cmd.token_count);
    STRCMP_EQUAL("100", cmd.tokens[1]);
    STRCMP_EQUAL("200", cmd.tokens[2]);
}

TEST(ShellTests, ParseTabs)
{
    strcpy(get_shell_buffer(), "motor\t100\t200");

    struct command cmd{parse_cli_buffer_contents()};

    LONGS_EQUAL(3u, cmd.token_count);
    STRCMP_EQUAL("100", cmd.tokens[1]);
    STRCMP_EQUAL("200", cmd.tokens[2]);
}

TEST(ShellTests, ParseStopsAtMaxParameterCount)
{
    strcpy(get_shell_buffer(), "cmd 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16");

    struct command cmd{parse_cli_buffer_contents()};

    LONGS_EQUAL(MAX_PARAMETER_COUNT + 1u, cmd.token_count);
}

TEST(ShellTests, PollShellIgnoresEmptyLine)
{
    feedCharacters("\n");

    poll_shell();

    STRCMP_EQUAL("", get_shell_buffer());
    LONGS_EQUAL(0u, get_shell_buffer_size());
    CHECK_FALSE(get_ready_for_parsing());
}

TEST(ShellTests, PollShellResetsStateAfterCommand)
{
    feedCharacters("help\r");

    mock().ignoreOtherCalls();

    poll_shell();

    STRCMP_EQUAL("", get_shell_buffer());
    LONGS_EQUAL(0u, get_shell_buffer_size());
    CHECK_FALSE(get_ready_for_parsing());
}
