/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : test_device.c                                         */
/*                                                                            */
/* Implementation for micromouse test_device library                          */
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
#include "navigation.h"
#include "command.h"
#include "test_device.h"

/*----------------------------------------------------------------------------*/
/*                         Private Function Prototypes                        */
/*----------------------------------------------------------------------------*/
static void print_move_forward_statistics(struct move_forward_statistics stats);
static void print_rotate_statistics(struct rotate_statistics stats);

/*----------------------------------------------------------------------------*/
/*                               Private Globals                              */
/*----------------------------------------------------------------------------*/
enum
{
    TEST_COMMAND_TOKEN_COUNT = 2,
    TEST_IR_COMMAND_TOKEN_COUNT = 3,
    TEST_WHEEL_ENCODER_COMMAND_TOKEN_COUNT = 3,
    TEST_NAVIGATE_COMMAND_TOKEN_COUNT = 3,
};

enum
{
    IR_DISTANCE_PARAMETER_COUNT = 5,
    IR_FREE_PARAMETER_COUNT = 2,
    IR_SPEED_PARAMETER_COUNT = 1,

    WHEEL_ENCODER_TARGET_PARAMETER_COUNT = 6,
    WHEEL_ENCODER_DECEL_PARAMETER_COUNT = 6
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

static const struct command_node test_wheel_encoder_commands[] =
{
    {
        .name = "target",
        .help =
            "Run wheel encoder target test: "
            "timeout_ms drift_delay_ms encoder_target "
            "start_speed end_speed speed_step",
        .validate = validate_test_wheel_encoder_target,
        .execute = execute_test_wheel_encoder_target
    },
    {
        .name = "deceleration",
        .help =
            "Run wheel encoder deceleration test: "
            "timeout_ms drift_delay_ms encoder_target "
            "start_speed top_speed max_accel_decel_percent",
        .validate = validate_test_wheel_encoder_deceleration,
        .execute = execute_test_wheel_encoder_deceleration
    }
};

static const struct command_node test_navigate_commands[] =
{
    {
        .name = "move-forward",
        .help = "Execute one-cell move forward",
        .validate = validate_test_navigate_move_forward,
        .execute = execute_test_navigate_move_forward
    },
    {
        .name = "rotate-clockwise-90",
        .help = "Execute clockwise 90 degree rotation",
        .validate = validate_test_navigate_rotate_clockwise_90,
        .execute = execute_test_navigate_rotate_clockwise_90
    },
    {
        .name = "rotate-counterclockwise-90",
        .help = "Execute counter-clockwise 90 degree rotation",
        .validate = validate_test_navigate_rotate_counterclockwise_90,
        .execute = execute_test_navigate_rotate_counterclockwise_90
    },
    {
        .name = "rotate-180",
        .help = "Execute 180 degree rotation",
        .validate = validate_test_navigate_rotate_180,
        .execute = execute_test_navigate_rotate_180
    },
    {
        .name = "left-wall-presence",
        .help = "Check left wall presence",
        .validate = validate_test_navigate_left_wall_presence,
        .execute = execute_test_navigate_left_wall_presence
    },
    {
        .name = "right-wall-presence",
        .help = "Check right wall presence",
        .validate = validate_test_navigate_right_wall_presence,
        .execute = execute_test_navigate_right_wall_presence
    },
    {
        .name = "front-wall-presence",
        .help = "Check front wall presence",
        .validate = validate_test_navigate_front_wall_presence,
        .execute = execute_test_navigate_front_wall_presence
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
    },
    {
        .name = "wheel-encoder",
        .help = "Run wheel motor and encoder tests",
        .validate = validate_test_wheel_encoder,
        .execute = NULL,
        .children = test_wheel_encoder_commands,
        .child_count = sizeof(test_wheel_encoder_commands) / sizeof(test_wheel_encoder_commands[0])
    },
    {
        .name = "vacuum",
        .help = "Run vacuum self-test",
        .validate = validate_test_vacuum,
        .execute = execute_test_vacuum
    },
    {
        .name = "navigate",
        .help = "Run navigation tests",
        .validate = validate_test_navigate,
        .execute = NULL,
        .children = test_navigate_commands,
        .child_count = sizeof(test_navigate_commands) / sizeof(test_navigate_commands[0])
    }
};

static const struct command_node test_node =
{
    .name = "test",
    .help = "Run device self-tests",
    .validate = validate_test,
    .children = test_commands,
    .child_count = sizeof(test_commands) / sizeof(test_commands[0])
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Definitions                        */
/*----------------------------------------------------------------------------*/
const struct command_node *get_test_node(void)
{
    return &test_node;
}

const struct command_node *get_test_commands(void)
{
    return test_commands;
}

uint32_t get_test_commands_count(void)
{
    return sizeof(test_commands) / sizeof(test_commands[0]);
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
/* test wheel-encoder */
enum validation_result validate_test_wheel_encoder(struct command *cmd)
{
    if (cmd->token_count == TEST_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder target */
enum validation_result validate_test_wheel_encoder_target(struct command *cmd)
{
    uint32_t expected =
        TEST_WHEEL_ENCODER_COMMAND_TOKEN_COUNT + WHEEL_ENCODER_TARGET_PARAMETER_COUNT;

    if (cmd->token_count < expected) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > expected) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_wheel_encoder_target(struct command const *cmd)
{
    uint32_t base = TEST_WHEEL_ENCODER_COMMAND_TOKEN_COUNT;

    struct wheel_motor_and_encoder_test_config cfg;
    cfg.timeout_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.drift_delay_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.encoder_target = (int32_t)strtol(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.start_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.end_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);
    cfg.speed_step = (uint8_t)strtoul(cmd->tokens[base + PARAM_5_OFFSET], NULL, 10);

    printf("running wheel encoder target test...\r\n");
    wheel_motor_and_encoder_test(cfg);
    printf("ending wheel encoder target test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder deceleration */
enum validation_result validate_test_wheel_encoder_deceleration(struct command *cmd)
{
    uint32_t expected =
        TEST_WHEEL_ENCODER_COMMAND_TOKEN_COUNT + WHEEL_ENCODER_DECEL_PARAMETER_COUNT;

    if (cmd->token_count < expected) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    if (cmd->token_count > expected) {
        return COMMAND_VALIDATION_TOO_MANY_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

void execute_test_wheel_encoder_deceleration(struct command const *cmd)
{
    uint32_t base = TEST_WHEEL_ENCODER_COMMAND_TOKEN_COUNT;

    struct wheel_motor_deceleration_test_config cfg;
    cfg.timeout_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_0_OFFSET], NULL, 10);
    cfg.drift_delay_ms = (uint32_t)strtoul(cmd->tokens[base + PARAM_1_OFFSET], NULL, 10);
    cfg.encoder_target = (int32_t)strtol(cmd->tokens[base + PARAM_2_OFFSET], NULL, 10);
    cfg.start_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_3_OFFSET], NULL, 10);
    cfg.top_speed = (uint8_t)strtoul(cmd->tokens[base + PARAM_4_OFFSET], NULL, 10);
    cfg.max_accel_decel_percent = (uint8_t)strtoul(cmd->tokens[base + PARAM_5_OFFSET], NULL, 10);

    printf("running wheel encoder deceleration test...\r\n");
    wheel_motor_deceleration_test(cfg);
    printf("ending wheel encoder deceleration test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test vacuum */
enum validation_result validate_test_vacuum(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_COMMAND_TOKEN_COUNT);
}

void execute_test_vacuum(struct command const *cmd)
{
    (void)cmd;

    printf("running vacuum test...\r\n");
    vacuum_test();
    printf("ending vacuum test...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test navigate */
enum validation_result validate_test_navigate(struct command *cmd)
{
    if (cmd->token_count == TEST_COMMAND_TOKEN_COUNT) {
        return COMMAND_VALIDATION_TOO_FEW_PARAMETERS;
    }

    return COMMAND_VALIDATION_SUCCESS;
}

/*----------------------------------------------------------------------------*/
/* test navigate move forward */
enum validation_result validate_test_navigate_move_forward(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_move_forward(struct command const *cmd)
{
    (void)cmd;

    printf("running move-forward...\r\n");
    move_forward();
    print_move_forward_statistics(get_move_forward_statistics());
    printf("ending move-forward...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate clockwise 90 */
enum validation_result validate_test_navigate_rotate_clockwise_90(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_rotate_clockwise_90(struct command const *cmd)
{
    (void)cmd;

    printf("running rotate-clockwise-90...\r\n");
    rotate_clockwise_90_deg();
    print_rotate_statistics(get_rotate_statistics());
    printf("ending rotate-clockwise-90...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate counterclockwise 90 */
enum validation_result validate_test_navigate_rotate_counterclockwise_90(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_rotate_counterclockwise_90(struct command const *cmd)
{
    (void)cmd;

    printf("running rotate-counterclockwise-90...\r\n");
    rotate_counter_clockwise_90_deg();
    print_rotate_statistics(get_rotate_statistics());
    printf("ending rotate-counterclockwise-90...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate 180 */
enum validation_result validate_test_navigate_rotate_180(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_rotate_180(struct command const *cmd)
{
    (void)cmd;

    printf("running rotate-180...\r\n");
    rotate_180_deg();
    print_rotate_statistics(get_rotate_statistics());
    printf("ending rotate-180...\r\n");
}

/*----------------------------------------------------------------------------*/
/* test navigate left-wall presence */
enum validation_result validate_test_navigate_left_wall_presence(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_left_wall_presence(struct command const *cmd)
{
    (void)cmd;

    printf("left_wall_present=%s\r\n", is_left_wall_present() ? "true" : "false");
}

/*----------------------------------------------------------------------------*/
/* test navigate right-wall presence*/
enum validation_result validate_test_navigate_right_wall_presence(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_right_wall_presence(struct command const *cmd)
{
    (void)cmd;

    printf("right_wall_present=%s\r\n", is_right_wall_present() ? "true" : "false");
}

/*----------------------------------------------------------------------------*/
/* test navigate front-wall presence */
enum validation_result validate_test_navigate_front_wall_presence(struct command *cmd)
{
    return validate_parameterless_command(cmd, TEST_NAVIGATE_COMMAND_TOKEN_COUNT);
}

void execute_test_navigate_front_wall_presence(struct command const *cmd)
{
    (void)cmd;

    printf("front_wall_present=%s\r\n", is_front_wall_present() ? "true" : "false");
}

/*----------------------------------------------------------------------------*/
/*                        Private Function Definitions                        */
/*----------------------------------------------------------------------------*/
static void print_move_forward_statistics(struct move_forward_statistics stats)
{
    printf("control_loop_iterations=%" PRIu32 "\r\n", stats.control_loop_iterations);
    printf("final_encoder_1_ticks=%" PRId32 "\r\n", stats.final_encoder_1_ticks);
    printf("final_encoder_2_ticks=%" PRId32 "\r\n", stats.final_encoder_2_ticks);
    printf("left_wall_present=%s\r\n", stats.left_wall_present ? "true" : "false");
    printf("right_wall_present=%s\r\n", stats.right_wall_present ? "true" : "false");
    printf("timeout_occurred=%s\r\n", stats.timeout_occurred ? "true" : "false");
}

static void print_rotate_statistics(struct rotate_statistics stats)
{
    printf("control_loop_iterations=%" PRIu32 "\r\n", stats.control_loop_iterations);
    printf("final_encoder_1_ticks=%" PRId32 "\r\n", stats.final_encoder_1_ticks);
    printf("final_encoder_2_ticks=%" PRId32 "\r\n", stats.final_encoder_2_ticks);
    printf("timeout_occurred=%s\r\n", stats.timeout_occurred ? "true" : "false");
}
