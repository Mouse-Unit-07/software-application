/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : build_info.c                                          */
/*                                                                            */
/* Implementation for micromouse build_info library                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Include Files                                */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "command.h"
#include "build_info.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
static struct command_node const build_info_node = {
        .name = "build-info",
        .help = "Display firmware build information",
        .parameters = NULL,
        .validate = validate_build,
        .execute = execute_build};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
struct command_node const *get_build_info_node(void)
{
    return &build_info_node;
}

enum validation_result validate_build(struct command *cmd)
{
    return validate_parameterless_command(cmd, ROOT_COMMAND_TOKEN_COUNT);
}

void execute_build(struct command const *cmd)
{
    (void)cmd;

    printf("Build Information\r\n");
    printf("-----------------\r\n");

    printf("Date         : %s\r\n", build_date);
    printf("Time         : %s\r\n", build_time);
    printf("Configuration: %s\r\n", build_configuration);
    printf("Mouse Target : %s\r\n", build_mouse_target);
    printf("\r\n");

    printf("Repositories\r\n");
    printf("-----------------\r\n");

    for (uint32_t i = 0u; i < build_repository_count; i++) {
        printf("%s\r\n", build_repositories[i].name);
        printf("    URL    : %s\r\n", build_repositories[i].url);
        printf("    Commit : %s\r\n", build_repositories[i].commit);
        printf("\r\n");
    }
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
/* none */
