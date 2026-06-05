/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : core.c                                                */
/*                                                                            */
/* Implementation for micromouse core library                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "user_interface.h"
#include "commands.h"
#include "core.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void reset_shell_state(void);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    MAX_BUFFER_SIZE = 128
};

static char shell_buffer[MAX_BUFFER_SIZE] = {0};
static uint32_t shell_buffer_size = 0u;
static bool ready_for_parsing = false;

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
void init_shell(void)
{
    reset_shell_state();
}

void deinit_shell(void)
{
    reset_shell_state();
}

void poll_shell(void)
{
    load_cli_buffer_contents();
    if (ready_for_parsing) {
        struct command cmd = parse_cli_buffer_contents();
        if (cmd.command[0] != '\0') {
            process_command(&cmd);
        }
        reset_shell_state();
    }
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static void reset_shell_state(void)
{
    memset(shell_buffer, 0, sizeof(shell_buffer));
    shell_buffer_size = 0U;
    ready_for_parsing = false;
}

void load_cli_buffer_contents(void)
{
    while (!is_cli_buffer_empty()) {
        char c = pop_cli_buffer();

        switch (c) {
            case '\b':
                /* delete character for backspace key */
                if (shell_buffer_size > 0U) {
                    shell_buffer_size--;
                    shell_buffer[shell_buffer_size] = '\0';
                }
                break;
            case '\r':
            case '\n':
                /* flag for parsing for ENTER key */
                shell_buffer[shell_buffer_size] = '\0';
                ready_for_parsing = true;
                clear_cli_buffer();
                return;

            default:
                if (shell_buffer_size >= (MAX_BUFFER_SIZE - 1u)) {
                    printf("shell overflow error\r\n");
                    reset_shell_state();
                    clear_cli_buffer();
                    return;
                }
                shell_buffer[shell_buffer_size] = c;
                shell_buffer_size++;
                break;
        }
    }
}

struct command parse_cli_buffer_contents(void)
{
    struct command cmd = {{0}};
    char *save_ptr = NULL;
    char *token = strtok_r(shell_buffer, " \t", &save_ptr);

    if (token == NULL) {
        return cmd;
    }

    strncpy(cmd.command, token, MAX_COMMAND_SIZE - 1U);
    cmd.command[MAX_COMMAND_SIZE - 1u] = '\0';

    while (cmd.parameter_count < MAX_PARAMETER_COUNT) {
        token = strtok_r(NULL, " \t", &save_ptr);
        if (token == NULL) {
            break;
        }
        cmd.parameters[cmd.parameter_count] = token;
        cmd.parameter_count++;
    }

    return cmd;
}

void process_command(struct command const *cmd)
{
    for (uint32_t i = 0u; i < get_command_table_size(); i++) {
        struct command_table_entry entry = get_command_table_entry_at_index(i);
        enum validation_result result = entry.validate(cmd);

        switch (result) {
            case COMMAND_VALIDATION_SUCCESS:
                entry.execute(cmd);
                return;

            case COMMAND_VALIDATION_BAD_PARAMETER:
                printf("Invalid parameter: %s\r\n", cmd->parameters[cmd->bad_parameter_index]);
                return;

            case COMMAND_VALIDATION_TOO_MANY_PARAMETERS:
                printf("%s: Too many parameters\r\n", cmd->command);
                return;

            case COMMAND_VALIDATION_TOO_FEW_PARAMETERS:
                printf("%s: Missing parameters\r\n", cmd->command);
                return;

            case COMMAND_VALIDATION_NOT_MATCHED:
                break;

            default:
                printf("Command validation error\r\n");
                break;
        }
    }

    printf("Unrecognized command\r\n");
}

char *get_shell_buffer(void)
{
    return shell_buffer;
}

uint32_t get_shell_buffer_size(void)
{
    return shell_buffer_size;
}

bool get_ready_for_parsing(void)
{
    return ready_for_parsing;
}
