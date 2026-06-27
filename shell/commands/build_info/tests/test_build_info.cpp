/*================================ FILE INFO =================================*/
/* Filename           : test_build_info.cpp                                   */
/*                                                                            */
/* Test implementation for build_info.c                                       */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdint.h>
#include "command.h"
#include "build_info.h"

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

enum validation_result validate_parameterless_command(struct command const *cmd,
                                                      uint32_t token_count)
{
    return (enum validation_result)mock()
            .actualCall("validate_parameterless_command")
            .withPointerParameter("cmd", (void *)cmd)
            .withUnsignedIntParameter("token_count", token_count)
            .returnIntValue();
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(BuildInfoTests)
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
TEST(BuildInfoTests, GetBuildInfoNodeReturnsExpectedNode)
{
    struct command_node const *node{get_build_info_node()};

    POINTERS_EQUAL(validate_build, node->validate);
    POINTERS_EQUAL(execute_build, node->execute);

    STRCMP_EQUAL("build-info", node->name);
    STRCMP_EQUAL("Display firmware build information", node->help);

    POINTERS_EQUAL(NULL, node->parameters);
    POINTERS_EQUAL(NULL, node->children);
    UNSIGNED_LONGS_EQUAL(0u, node->child_count);
}

TEST(BuildInfoTests, ValidateBuildDelegatesToCommonValidator)
{
    struct command cmd{0};

    mock().expectOneCall("validate_parameterless_command")
            .withPointerParameter("cmd", &cmd)
            .withUnsignedIntParameter("token_count", ROOT_COMMAND_TOKEN_COUNT)
            .andReturnValue(COMMAND_VALIDATION_SUCCESS);

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_build(&cmd));
}

TEST(BuildInfoTests, ValidateBuildReturnsValidatorFailure)
{
    struct command cmd{0};

    mock().expectOneCall("validate_parameterless_command")
            .withPointerParameter("cmd", &cmd)
            .withUnsignedIntParameter("token_count", ROOT_COMMAND_TOKEN_COUNT)
            .andReturnValue(COMMAND_VALIDATION_TOO_MANY_PARAMETERS);

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_build(&cmd));
}

TEST(BuildInfoTests, GeneratedBuildStringsExist)
{
    CHECK(strlen(build_date) > 0);
    CHECK(strlen(build_time) > 0);
    CHECK(strlen(build_configuration) > 0);
    CHECK(strlen(build_mouse_target) > 0);
}

TEST(BuildInfoTests, RepositoryInformationLooksValid)
{
    CHECK(build_repository_count > 0);

    for (uint32_t i{0u}; i < build_repository_count; i++) {
        CHECK(build_repositories[i].name != NULL);
        CHECK(build_repositories[i].url != NULL);
        CHECK(build_repositories[i].commit != NULL);

        CHECK(strlen(build_repositories[i].name) > 0);
        CHECK(strlen(build_repositories[i].url) > 0);
        CHECK(strlen(build_repositories[i].commit) > 0);
    }
}

TEST(BuildInfoTests, ExecuteBuildDoesNotCrash)
{
    struct command cmd = {0};

    execute_build(&cmd);
}
