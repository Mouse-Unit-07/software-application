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
#include <stdlib.h>
#include <string.h>
#include "device_self_tests.h"
#include "fault_detector.h"
#include "global_time.h"
#include "maze_solver_common.h"
#include "configuration.h"
#include "wall_follower.h"
#include "partial_flood_fill.h"
#include "commands.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static enum validation_result validate_parameterless_command(struct command const *cmd,
                                                             uint32_t token_count);
static void print_help_recursive(struct command_node const *nodes, uint32_t count,
                                 char const *prefix);

static void print_maze_solver_config(struct maze_solver_config cfg);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    ROOT_COMMAND_TOKEN_COUNT = 1,
    TEST_COMMAND_TOKEN_COUNT = 2,
    TEST_IR_COMMAND_TOKEN_COUNT = 3,
    GET_COMMAND_TOKEN_COUNT = 2,
    SET_COMMAND_TOKEN_COUNT = 2,
    SOLVE_COMMAND_TOKEN_COUNT = 2,
};

enum
{
    PARAM_0_OFFSET = 0,
    PARAM_1_OFFSET,
    PARAM_2_OFFSET,
    PARAM_3_OFFSET,
    PARAM_4_OFFSET,
    PARAM_5_OFFSET
};

enum
{
    MAZE_SOLVER_PARAMETER_COUNT = 5,

    WALLFOLLOWER_PARAM_COUNT_MIN = 1,
    WALLFOLLOWER_PARAM_COUNT_MAX = 2,
    FLOODFILL_PARAM_COUNT_MIN = 0,
    FLOODFILL_PARAM_COUNT_MAX = 1,

    IR_DISTANCE_PARAMETER_COUNT = 5,
    IR_FREE_PARAMETER_COUNT = 2,
    IR_SPEED_PARAMETER_COUNT = 1
};

static const struct command_node test_ir_commands[] =
{
    {
        .name = "distance",
        .help =
            "Run IR distance test: "
            "start_distance_cm end_distance_cm "
            "trials_per_distance time_per_trial_ms "
            "setup_delay_ms",
        .validate = validate_test_ir_distance,
        .execute = execute_test_ir_distance
    },
    {
        .name = "free",
        .help =
            "Run IR free-reading test: "
            "time_per_sensor_ms setup_delay_ms",
        .validate = validate_test_ir_free,
        .execute = execute_test_ir_free
    },
    {
        .name = "speed",
        .help =
            "Run IR read-speed test: "
            "time_per_sensor_ms",
        .validate = validate_test_ir_speed,
        .execute = execute_test_ir_speed
    }
};

static const struct command_node test_commands[] =
{
    {
        .name = "processor",
        .help = "Run processor self-test",
        .validate = validate_test_processor,
        .execute = execute_test_processor
    },
    {
        .name = "battery",
        .help = "Run battery comparator self-test",
        .validate = validate_test_battery,
        .execute = execute_test_battery
    },
    {
        .name = "enabler",
        .help = "Run power enabler self-test",
        .validate = validate_test_enabler,
        .execute = execute_test_enabler
    },
    {
        .name = "led",
        .help = "Run LED self-test",
        .validate = validate_test_led,
        .execute = execute_test_led
    },
    {
        .name = "pushbutton",
        .help = "Run pushbutton self-test",
        .validate = validate_test_pushbutton,
        .execute = execute_test_pushbutton
    },
    {
        .name = "ir",
        .help = "Run IR sensor tests",
        .validate = validate_test_ir,
        .execute = NULL,
        .children = test_ir_commands,
        .child_count = sizeof(test_ir_commands) / sizeof(test_ir_commands[0])
    }
};

static const struct command_node get_commands[] =
{
    {
        .name = "solver-default",
        .help = "Display default solver configuration",
        .validate = validate_get_solver_default,
        .execute = execute_get_solver_default
    },
    {
        .name = "solver-test",
        .help = "Display test solver configuration",
        .validate = validate_get_solver_test,
        .execute = execute_get_solver_test
    },
    {
        .name = "solver-current",
        .help = "Display current solver configuration",
        .validate = validate_get_solver_current,
        .execute = execute_get_solver_current
    }
};

static const struct command_node set_commands[] =
{
    {
        .name = "solver-default",
        .help = "Use default solver configuration",
        .validate = validate_set_solver_default,
        .execute = execute_set_solver_default
    },
    {
        .name = "solver-test",
        .help = "Use test solver configuration; optionally pass: "
            "maze_size, total_timeout_sec, move_forward_time_sec, "
            "rotate_90_deg_time_sec, rotate_180_deg_time_sec",
        .validate = validate_set_solver_test,
        .execute = execute_set_solver_test
    }
};

static const struct command_node solve_commands[] =
{
    {
        .name = "wallfollower",
        .help = "Run wall follower: left|right [enable]",
        .validate = validate_solve_wallfollower,
        .execute = execute_solve_wallfollower
    },
    {
        .name = "floodfill",
        .help = "Run partial flood fill [enable]",
        .validate = validate_solve_floodfill,
        .execute = execute_solve_floodfill
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
    {
        .name = "get",
        .help = "Read configuration values",
        .validate = validate_get,
        .execute = NULL,
        .children = get_commands,
        .child_count = sizeof(get_commands) / sizeof(get_commands[0])
    },
    {
        .name = "set",
        .help = "Choose configuration values to use w/ or w/o new values for test configs",
        .validate = validate_set,
        .execute = NULL,
        .children = set_commands,
        .child_count = sizeof(set_commands) / sizeof(set_commands[0])
    },
    {
        .name = "solve",
        .help = "Run maze solving algorithms",
        .validate = validate_solve,
        .execute = NULL,
        .children = solve_commands,
        .child_count = sizeof(solve_commands) / sizeof(solve_commands[0])
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
enum validation_result validate_help(struct command *cmd)
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
enum validation_result validate_clear(struct command *cmd)
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
enum validation_result validate_hardware_faults(struct command *cmd)
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
enum validation_result validate_get_time(struct command *cmd)
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
enum validation_result validate_test(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* test processor */
enum validation_result validate_test_processor(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_processor(struct command const *cmd)
{
    (void)cmd; /* unused due to no parameters */

    printf("running processor test...\r\n");
    processor_test();
    printf("ending processor test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test battery */
enum validation_result validate_test_battery(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_battery(struct command const *cmd)
{
    (void)cmd;

    printf("running battery test...\r\n");
    battery_comparator_test();
    printf("ending battery test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test enabler */
enum validation_result validate_test_enabler(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_enabler(struct command const *cmd)
{
    (void)cmd;

    printf("running enabler test...\r\n");
    power_enabler_test();
    printf("ending enabler test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test led */
enum validation_result validate_test_led(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_led(struct command const *cmd)
{
    (void)cmd;

    printf("running led test...\r\n");
    led_test();
    printf("ending led test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test pushbutton */
enum validation_result validate_test_pushbutton(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_pushbutton(struct command const *cmd)
{
    (void)cmd;

    printf("running pushbutton test...\r\n");
    pushbutton_test();
    printf("ending pushbutton test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test ir */
enum validation_result validate_test_ir(struct command *cmd)
{
    if (cmd->token_count == TEST_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* test IR distance */
enum validation_result validate_test_ir_distance(struct command *cmd)
{
    uint32_t expected = TEST_IR_COMMAND_TOKEN_COUNT + IR_DISTANCE_PARAMETER_COUNT;

    if (cmd->token_count < expected) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > expected) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_ir_distance(struct command const *cmd)
{
    uint32_t base = TEST_IR_COMMAND_TOKEN_COUNT;

    struct ir_distance_test_config cfg;
    cfg.start_distance_cm = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.end_distance_cm = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.trials_per_distance = (uint32_t)strtoul(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.time_per_trial_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.setup_delay_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);

    printf("running IR sensors distance test...\r\n");
    infrared_sensors_distance_test(cfg);
    printf("ending IR sensors distance test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test IR free */
enum validation_result validate_test_ir_free(struct command *cmd)
{
    uint32_t expected = TEST_IR_COMMAND_TOKEN_COUNT + IR_FREE_PARAMETER_COUNT;

    if (cmd->token_count < expected) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > expected) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_ir_free(struct command const *cmd)
{
    uint32_t base = TEST_IR_COMMAND_TOKEN_COUNT;

    struct ir_free_reading_test_config cfg;
    cfg.time_per_sensor_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.setup_delay_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);

    printf("running IR sensors free reading test...\r\n");
    infrared_sensors_free_reading_test(cfg);
    printf("ending IR sensors free reading test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test IR speed */
enum validation_result validate_test_ir_speed(struct command *cmd)
{
    uint32_t expected = TEST_IR_COMMAND_TOKEN_COUNT + IR_SPEED_PARAMETER_COUNT;

    if (cmd->token_count < expected) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > expected) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_ir_speed(struct command const *cmd)
{
    uint32_t base = TEST_IR_COMMAND_TOKEN_COUNT;

    uint32_t time_per_sensor_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);

    printf("running IR sensors read speed test...\r\n");
    infrared_sensors_read_speed_test(time_per_sensor_ms);
    printf("ending IR sensors read speed test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* get */
enum validation_result validate_get(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* get solver-default */
enum validation_result validate_get_solver_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_default(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_default_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* get solver-test */
enum validation_result validate_get_solver_test(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_test(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_test_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* get solver-current */
enum validation_result validate_get_solver_current(struct command *cmd)
{
    return validate_parameterless_command(cmd, GET_COMMAND_TOKEN_COUNT);
}

void execute_get_solver_current(struct command const *cmd)
{
    (void)cmd;

    print_maze_solver_config(get_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* set */
enum validation_result validate_set(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* set solver-default */
enum validation_result validate_set_solver_default(struct command *cmd)
{
    return validate_parameterless_command(cmd, SET_COMMAND_TOKEN_COUNT);
}

void execute_set_solver_default(struct command const *cmd)
{
    (void)cmd;

    set_maze_solver_config(get_default_maze_solver_config());
}

/*----------------------------------------------------------------------------*/
/* set solver-test */
enum validation_result validate_set_solver_test(struct command *cmd)
{
    uint32_t edit_token_count = SET_COMMAND_TOKEN_COUNT + MAZE_SOLVER_PARAMETER_COUNT;

    if ((cmd->token_count != SET_COMMAND_TOKEN_COUNT) && (cmd->token_count != edit_token_count)) {
        if (cmd->token_count < edit_token_count) {
            return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
        }

        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == edit_token_count) {
        struct maze_solver_config cfg;
        cfg.maze_size = (uint32_t)strtoul(cmd->tokens[param_0_offset], NULL, 10);

        if (cfg.maze_size > 16u) {
            cmd->bad_parameter_index = param_0_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_set_solver_test(struct command const *cmd)
{
    if (cmd->token_count == SET_COMMAND_TOKEN_COUNT) {
        set_maze_solver_config(get_test_maze_solver_config());
        return;
    }

    uint32_t base_offset = SET_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;
    uint32_t param_2_offset = base_offset + PARAM_2_OFFSET;
    uint32_t param_3_offset = base_offset + PARAM_3_OFFSET;
    uint32_t param_4_offset = base_offset + PARAM_4_OFFSET;

    struct maze_solver_config cfg;
    cfg.maze_size = (uint32_t)strtoul(cmd->tokens[param_0_offset], NULL, 10);
    cfg.total_timeout_sec = (uint32_t)strtoul(cmd->tokens[param_1_offset], NULL, 10);
    cfg.move_forward_time_sec = (uint32_t)strtoul(cmd->tokens[param_2_offset], NULL, 10);
    cfg.rotate_90_deg_time_sec = (uint32_t)strtoul(cmd->tokens[param_3_offset], NULL, 10);
    cfg.rotate_180_deg_time_sec = (uint32_t)strtoul(cmd->tokens[param_4_offset], NULL, 10);

    set_test_maze_solver_config(cfg);
}

/*----------------------------------------------------------------------------*/
/* solve */
enum validation_result validate_solve(struct command *cmd)
{
    if (cmd->token_count == ROOT_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* solve wallfollower */
enum validation_result validate_solve_wallfollower(struct command *cmd)
{
    uint32_t min_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MIN;
    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MAX;

    if (cmd->token_count < min_token_count) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > max_token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;

    if ((strcmp(cmd->tokens[param_0_offset], "left") != 0)
        && (strcmp(cmd->tokens[param_0_offset], "right") != 0)) {
        cmd->bad_parameter_index = param_0_offset;
        return COMMAND_VALIDATION_BAD_PARAMETER;
    }

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_1_offset], "enable") != 0) {
            cmd->bad_parameter_index = param_1_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_solve_wallfollower(struct command const *cmd)
{
    bool enable_print = false;
    enum wall_follower_mode mode;

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;
    uint32_t param_1_offset = base_offset + PARAM_1_OFFSET;

    if (strcmp(cmd->tokens[param_0_offset], "left") == 0) {
        mode = WALL_FOLLOWER_LEFT;
    } else {
        mode = WALL_FOLLOWER_RIGHT;
    }

    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + WALLFOLLOWER_PARAM_COUNT_MAX;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_1_offset], "enable") == 0) {
            enable_print = true;
        }
    }

    run_wall_follower(mode, enable_print);
}

/*----------------------------------------------------------------------------*/
/* solve floodfill */
enum validation_result validate_solve_floodfill(struct command *cmd)
{
    uint32_t min_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MIN;
    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MAX;

    if (cmd->token_count < min_token_count) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > max_token_count) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_0_offset], "enable") != 0) {
            cmd->bad_parameter_index = param_0_offset;
            return COMMAND_VALIDATION_BAD_PARAMETER;
        }
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_solve_floodfill(struct command const *cmd)
{
    bool enable_print = false;

    uint32_t max_token_count = SOLVE_COMMAND_TOKEN_COUNT + FLOODFILL_PARAM_COUNT_MAX;
    uint32_t base_offset = SOLVE_COMMAND_TOKEN_COUNT;
    uint32_t param_0_offset = base_offset + PARAM_0_OFFSET;

    if (cmd->token_count == max_token_count) {
        if (strcmp(cmd->tokens[param_0_offset], "enable") == 0) {
            enable_print = true;
        }
    }

    run_partial_flood_fill(enable_print);
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

static void print_maze_solver_config(struct maze_solver_config cfg)
{
    printf("maze_size                = %" PRIu32 "\r\n", cfg.maze_size);
    printf("total_timeout_sec        = %" PRIu32 "\r\n", cfg.total_timeout_sec);
    printf("move_forward_time_sec    = %" PRIu32 "\r\n", cfg.move_forward_time_sec);
    printf("rotate_90_deg_time_sec   = %" PRIu32 "\r\n", cfg.rotate_90_deg_time_sec);
    printf("rotate_180_deg_time_sec  = %" PRIu32 "\r\n", cfg.rotate_180_deg_time_sec);
}
