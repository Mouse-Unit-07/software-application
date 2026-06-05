/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : commands.c                                            */
/*                                                                            */
/* Implementation for micromouse commands library                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "device_self_tests.h"
#include "fault_detector.h"
#include "global_time.h"
#include "commands.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_parameterless_command(struct command const *cmd,
                                                             uint32_t token_count);
static void print_help_recursive(struct command_node const *nodes, uint32_t count,
                                 char const *prefix);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    ROOT_COMMAND_TOKEN_COUNT = 1,
    TEST_COMMAND_TOKEN_COUNT = 2,
    TEST_SUBCOMMAND_TOKEN_COUNT = 3
};

static const struct command_node test_commands[] =
{
    {
        .name = "processor",
        .help = "Run processor self-test",
        .validate = validate_test_processor,
        .execute = execute_test_processor
    }
};

static const struct command_node root_commands[] =
{
    {
        .name = "help",
        .help = "Display available commands",
        .validate = validate_help,
        .execute = execute_help
    },
    {
        .name = "clear",
        .help = "Clear console",
        .validate = validate_clear,
        .execute = execute_clear
    },
    {
        .name = "faults",
        .help = "Display all hardware faults",
        .validate = validate_hardware_faults,
        .execute = execute_hardware_faults
    },
    {
        .name = "time",
        .help = "Display current time",
        .validate = validate_get_time,
        .execute = execute_get_time
    },
    {
        .name = "test",
        .help = "Run device self-tests",
        .validate = validate_test,
        .execute = NULL,
        .children = test_commands,
        .child_count = sizeof(test_commands) / sizeof(test_commands[0])
    },
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
struct command_node const *get_command_tree_root(void)
{
    return root_commands;
}

uint32_t get_command_tree_root_count(void)
{
    return sizeof(root_commands) / sizeof(root_commands[0]);
}

struct command_match find_command_node(struct command const *cmd)
{
    struct command_match match = {0};
    struct command_node const *current = get_command_tree_root();
    uint32_t count = get_command_tree_root_count();

    for (uint32_t token = 0u; token < cmd->token_count; token++) {
        struct command_node const *found = NULL;

        for (uint32_t i = 0u; i < count; i++) {
            if (strcmp(current[i].name, cmd->tokens[token]) == 0) {
                found = &current[i];
                current = found->children;
                count = found->child_count;
                break;
            }
        }

        if (found == NULL) {
            return match;
        }

        match.node = found;
        match.depth = token + 1u;
    }

    return match;
}

/*----------------------------------------------------------------------------*/
/* help */
enum validation_result validate_help(struct command const *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_help(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_help_recursive(get_command_tree_root(), get_command_tree_root_count(), "");
}

/*----------------------------------------------------------------------------*/
/* clear */
enum validation_result validate_clear(struct command const *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_clear(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("\e[1;1H\e[2J");
}

/*----------------------------------------------------------------------------*/
/* faults */
enum validation_result validate_hardware_faults(struct command const *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_hardware_faults(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    print_hardware_state();
}

/*----------------------------------------------------------------------------*/
/* time */
enum validation_result validate_get_time(struct command const *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_get_time(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    uint32_t time_sec = get_current_global_time_sec();
    printf("time passed since init: %" PRIu32 " min, %" PRIu32 " sec\r\n", time_sec / 60,
           time_sec % 60);
}

/*----------------------------------------------------------------------------*/
/* test */
enum validation_result validate_test(struct command const *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* test processor */
enum validation_result validate_test_processor(struct command const *cmd)
{
    if (cmd->token_count != TEST_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_processor(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("running processor test...\r\n");
    processor_test();
    printf("ending processor test...\r\n");
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_parameterless_command(struct command const *cmd,
                                                             uint32_t token_count)
{
    if (cmd->token_count > token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

static void print_help_recursive(struct command_node const *nodes, uint32_t count,
                                 char const *prefix)
{
    char command_name[MAX_COMMAND_NAME_SIZE];

    for (uint32_t i = 0u; i < count; i++) {
        if ((prefix != NULL) && (prefix[0] != '\0')) {
            snprintf(command_name, sizeof(command_name), "%s %s", prefix, nodes[i].name);
        } else {
            snprintf(command_name, sizeof(command_name), "%s", nodes[i].name);
        }

        printf("%-24s %s\r\n", command_name, nodes[i].help);

        if (nodes[i].child_count > 0u) {
            print_help_recursive(nodes[i].children, nodes[i].child_count, command_name);
        }
    }
}
