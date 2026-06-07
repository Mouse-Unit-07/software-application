/*================================ FILE INFO =================================*/
/* Filename           : test_command.cpp                                      */
/*                                                                            */
/* Test implementation for command.c                                          */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdint.h>
#include "command.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
struct command_node const grandchildren[] =
{
    {
        .name = "leaf"
    }
};

struct command_node const children[] =
{
    {
        .name = "child",
        .children = grandchildren,
        .child_count = 1
    }
};

struct command_node const roots[] =
{
    {
        .name = "root",
        .children = children,
        .child_count = 1
    }
};

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(CommandTests)
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
TEST(CommandTests, ValidateParameterlessCommandReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_parameterless_command(&cmd, 2));
}

TEST(CommandTests, ValidateParameterlessCommandReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_parameterless_command(&cmd, 2));
}

TEST(CommandTests, FindCommandNodeReturnsNullForEmptyCommand)
{
    struct command cmd{{0}};
    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(nullptr, match.node);
    LONGS_EQUAL(0u, match.depth);
}

TEST(CommandTests, FindCommandNodeReturnsRootNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "root";

    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(&roots[0], match.node);
    LONGS_EQUAL(1u, match.depth);
}

TEST(CommandTests, FindCommandNodeReturnsChildNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "root";
    cmd.tokens[1] = "child";

    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(&children[0], match.node);
    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandTests, FindCommandNodeReturnsGrandchildNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "root";
    cmd.tokens[1] = "child";
    cmd.tokens[2] = "leaf";

    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(&grandchildren[0], match.node);
    LONGS_EQUAL(3u, match.depth);
}

TEST(CommandTests, FindCommandNodeReturnsNullForUnknownCommand)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "bad";

    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(nullptr, match.node);
    LONGS_EQUAL(0u, match.depth);
}

TEST(CommandTests, FindCommandNodeReturnsLastValidMatch)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "root";
    cmd.tokens[1] = "child";
    cmd.tokens[2] = "bad";

    struct command_match match = find_command_node(&cmd, roots, 1);

    POINTERS_EQUAL(&children[0], match.node);
    LONGS_EQUAL(2u, match.depth);
}
