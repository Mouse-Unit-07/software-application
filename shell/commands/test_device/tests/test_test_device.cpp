/*================================ FILE INFO =================================*/
/* Filename           : test_test_device.cpp                                  */
/*                                                                            */
/* Test implementation for test_device.c                                      */
/*                                                                            */
/*============================================================================*/

/*============================================================================*/
/*                               Include Files                                */
/*============================================================================*/
extern "C"
{

#include <stdbool.h>
#include <stdint.h>
#include "device_self_tests.h"
#include "navigation.h"
#include "command.h"
#include "test_device.h"

}

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*============================================================================*/
/*                             Public Definitions                             */
/*============================================================================*/
constexpr uint32_t FAKE_ROOT_COMMANDS_COUNT{1u};

struct command_node fake_root_commands[FAKE_ROOT_COMMANDS_COUNT]{{0}};

void init_test_fakes(void)
{
    fake_root_commands[0].name = "test";
    fake_root_commands[0].help = "Run device self-tests";
    fake_root_commands[0].validate = validate_test;
    fake_root_commands[0].execute = execute_test;
    fake_root_commands[0].children = get_test_commands();
    fake_root_commands[0].child_count = get_test_commands_count();
}

void deinit_test_fakes(void)
{
    fake_root_commands[0] = {};
}

struct command make_test_command(char const *subcommand, uint32_t token_count)
{
    struct command cmd{{0}};

    cmd.token_count = token_count;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = subcommand;

    return cmd;
}

void check_command_lookup(char const *command_name, uint32_t token_count)
{
    struct command cmd{make_test_command(command_name, token_count)};
    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL(command_name, node->name);
}

void check_validation_success(char const *command_name, uint32_t token_count,
                              enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_test_command(command_name, token_count)};
    enum validation_result actual = validate(&cmd);
    SimpleString msg = StringFromFormat("SUCCESS CHECK FAILED: command=%s token_count=%u actual=%d",
                                        command_name, token_count, actual);

    LONGS_EQUAL_TEXT(COMMAND_VALIDATION_SUCCESS, actual, msg.asCharString());
}

void check_validation_too_many_params(char const *command_name, uint32_t token_count,
                                      enum validation_result (*validate)(struct command *))
{
    struct command cmd{make_test_command(command_name, token_count)};
    enum validation_result actual = validate(&cmd);
    SimpleString msg =
        StringFromFormat("TOO_MANY_PARAMS CHECK FAILED: command=%s token_count=%u actual=%d",
                         command_name, token_count, actual);

    LONGS_EQUAL_TEXT(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, actual, msg.asCharString());
}

struct test_command_case {
    const char *name;
    enum validation_result (*validate)(struct command *);
};

static const test_command_case simple_commands[] = {
    {"processor", validate_test_processor},
    {"battery", validate_test_battery},
    {"enabler", validate_test_enabler},
    {"led", validate_test_led},
    {"ir", validate_test_ir},
    {"wheel-encoder", validate_test_wheel_encoder},
    {"pushbutton", validate_test_pushbutton},
    {"vacuum", validate_test_vacuum},
    {"navigate", validate_test_navigate}};

struct structured_command_case {
    const char *parent;
    const char *child;
    enum validation_result (*validate)(struct command *);
    uint32_t success_tokens;
    uint32_t too_few_tokens;
    uint32_t too_many_tokens;
};

static const structured_command_case ir_cases[] = {
    {"ir", "distance", validate_test_ir_distance, 8, 7, 9},
    {"ir", "free", validate_test_ir_free, 5, 4, 6},
    {"ir", "speed", validate_test_ir_speed, 4, 3, 5}};

static const structured_command_case wheel_encoder_cases[] = {
    {"wheel-encoder", "target", validate_test_wheel_encoder_target, 9, 7, 10},
    {"wheel-encoder", "drift", validate_test_wheel_encoder_drift, 9, 7, 10},
    {"wheel-encoder", "deceleration", validate_test_wheel_encoder_deceleration, 9, 7, 10}};

static const structured_command_case navigate_cases[] = {
    {"navigate", "move-forward", validate_test_navigate_move_forward, 3, 2, 4},
    {"navigate", "move-forward-continuous", validate_test_navigate_move_forward_continuous, 3, 2,
     4},
    {"navigate", "rotate-clockwise-90", validate_test_navigate_rotate_clockwise_90, 3, 2, 4},
    {"navigate", "rotate-counterclockwise-90", validate_test_navigate_rotate_counterclockwise_90, 3,
     2, 4},
    {"navigate", "rotate-180", validate_test_navigate_rotate_180, 3, 2, 4},
    {"navigate", "left-wall-presence", validate_test_navigate_left_wall_presence, 3, 2, 4},
    {"navigate", "right-wall-presence", validate_test_navigate_right_wall_presence, 3, 2, 4},
    {"navigate", "front-wall-presence", validate_test_navigate_front_wall_presence, 3, 2, 4}};

/*============================================================================*/
/*                            Mock Implementations                            */
/*============================================================================*/
extern "C"
{

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

void infrared_sensors_distance_test(struct ir_distance_test_config cfg)
{
    mock()
        .actualCall("infrared_sensors_distance_test")
        .withUnsignedIntParameter("start_distance_cm", cfg.start_distance_cm)
        .withUnsignedIntParameter("end_distance_cm", cfg.end_distance_cm)
        .withUnsignedIntParameter("trials_per_distance", cfg.trials_per_distance)
        .withUnsignedIntParameter("time_per_trial_ms", cfg.time_per_trial_ms)
        .withUnsignedIntParameter("setup_delay_ms", cfg.setup_delay_ms);
}

void infrared_sensors_free_reading_test(struct ir_free_reading_test_config cfg)
{
    mock().actualCall("infrared_sensors_free_reading_test")
        .withUnsignedIntParameter("time_per_sensor_ms", cfg.time_per_sensor_ms)
        .withUnsignedIntParameter("setup_delay_ms", cfg.setup_delay_ms);
}

void infrared_sensors_read_speed_test(uint32_t time_per_sensor_ms)
{
    mock().actualCall("infrared_sensors_read_speed_test")
        .withUnsignedIntParameter("time_per_sensor_ms", time_per_sensor_ms);
}

void wheel_motor_and_encoder_test(struct wheel_motor_and_encoder_test_config cfg)
{
    mock().actualCall("wheel_motor_and_encoder_test")
        .withUnsignedIntParameter("timeout_ms", cfg.timeout_ms)
        .withUnsignedIntParameter("drift_delay_ms", cfg.drift_delay_ms)
        .withIntParameter("encoder_target", cfg.encoder_target)
        .withUnsignedIntParameter("start_speed", cfg.start_speed)
        .withUnsignedIntParameter("end_speed", cfg.end_speed)
        .withUnsignedIntParameter("speed_step", cfg.speed_step);
}

void wheel_motor_drift_test(struct wheel_motor_and_encoder_test_config cfg)
{
    mock().actualCall("wheel_motor_drift_test")
        .withUnsignedIntParameter("timeout_ms", cfg.timeout_ms)
        .withUnsignedIntParameter("drift_delay_ms", cfg.drift_delay_ms)
        .withIntParameter("encoder_target", cfg.encoder_target)
        .withUnsignedIntParameter("start_speed", cfg.start_speed)
        .withUnsignedIntParameter("end_speed", cfg.end_speed)
        .withUnsignedIntParameter("speed_step", cfg.speed_step);
}

void wheel_motor_deceleration_test(struct wheel_motor_deceleration_test_config cfg)
{
    mock().actualCall("wheel_motor_deceleration_test")
        .withUnsignedIntParameter("timeout_ms", cfg.timeout_ms)
        .withUnsignedIntParameter("drift_delay_ms", cfg.drift_delay_ms)
        .withIntParameter("encoder_target", cfg.encoder_target)
        .withUnsignedIntParameter("start_speed", cfg.start_speed)
        .withUnsignedIntParameter("top_speed", cfg.top_speed)
        .withUnsignedIntParameter("max_accel_decel_percent", cfg.max_accel_decel_percent);
}

void vacuum_test(void)
{
    mock().actualCall("vacuum_test");
}

void move_forward(void)
{
    mock().actualCall("move_forward");
}

uint32_t move_forward_until_turn_or_intersection_and_return_steps(void)
{
    mock().actualCall("move_forward_until_turn_or_intersection_and_return_steps");

    return 3u;
}

struct move_forward_statistics get_move_forward_statistics(void)
{
    mock().actualCall("get_move_forward_statistics");

    struct move_forward_statistics stats{};
    return stats;
}

void rotate_clockwise_90_deg(void)
{
    mock().actualCall("rotate_clockwise_90_deg");
}

void rotate_counter_clockwise_90_deg(void)
{
    mock().actualCall("rotate_counter_clockwise_90_deg");
}

void rotate_180_deg(void)
{
    mock().actualCall("rotate_180_deg");
}

struct rotate_statistics get_rotate_statistics(void)
{
    mock().actualCall("get_rotate_statistics");

    struct rotate_statistics stats{};
    return stats;
}

bool is_left_wall_present(void)
{
    mock().actualCall("is_left_wall_present");
    return false;
}

bool is_right_wall_present(void)
{
    mock().actualCall("is_right_wall_present");
    return false;
}

bool is_front_wall_present(void)
{
    mock().actualCall("is_front_wall_present");
    return false;
}

}

/*============================================================================*/
/*                                 Test Group                                 */
/*============================================================================*/
TEST_GROUP(CommandsTests)
{
    void setup() override
    {
        init_test_fakes();
        mock().clear();
    }

    void teardown() override
    {
        mock().checkExpectations();
        mock().clear();
        deinit_test_fakes();
    }
};

/*============================================================================*/
/*                                    Tests                                   */
/*============================================================================*/
TEST(CommandsTests, GetTestNodeReturnsValidNode)
{
    const struct command_node *node = get_test_node();

    CHECK(node != nullptr);
    STRCMP_EQUAL("test", node->name);
    POINTERS_EQUAL(validate_test, node->validate);
    POINTERS_EQUAL(execute_test, node->execute);
    POINTERS_EQUAL(get_test_commands(), node->children);
    LONGS_EQUAL(get_test_commands_count(), node->child_count);
}

TEST(CommandsTests, GetTestCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(9u, get_test_commands_count());
}

TEST(CommandsTests, TestCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_test_commands();

    STRCMP_EQUAL("processor", commands[0].name);
    STRCMP_EQUAL("battery", commands[1].name);
    STRCMP_EQUAL("enabler", commands[2].name);
    STRCMP_EQUAL("led", commands[3].name);
    STRCMP_EQUAL("pushbutton", commands[4].name);
    STRCMP_EQUAL("ir", commands[5].name);
    STRCMP_EQUAL("wheel-encoder", commands[6].name);
    STRCMP_EQUAL("vacuum", commands[7].name);
    STRCMP_EQUAL("navigate", commands[8].name);
}

TEST(CommandsTests, TestCommandContainsSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(9u, node->child_count);
}

TEST(CommandsTests, FindCommandNodeReturnsExpectedNode)
{
    for (auto const &test : simple_commands) {
        check_command_lookup(test.name, 2);
    }
}

TEST(CommandsTests, ValidateCommandsReturnSuccess)
{
    for (auto const &test : simple_commands) {
        check_validation_success(test.name, 2, test.validate);
    }
}

TEST(CommandsTests, ValidateCommandsReturnTooManyParameters)
{
    for (auto const &test : simple_commands) {
        check_validation_too_many_params(test.name, 3, test.validate);
    }
}

/*----------------------------------------------------------------------------*/
/* test */
TEST(CommandsTests, FindCommandNodeReturnsTestNode)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("test", node->name);
}

TEST(CommandsTests, ValidateTestReturnsSuccess)
{
    struct command cmd{{0}};

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test(&cmd));
}

TEST(CommandsTests, ValidateTestReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test(&cmd));
}

TEST(CommandsTests, ExecuteTestRunsWithoutCrash)
{
    struct command cmd{{0}};

    execute_test(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test processor */
TEST(CommandsTests, TestProcessorCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    struct command_match match{
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, ExecuteTestProcessorCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("processor_test");

    execute_test_processor(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test battery */
TEST(CommandsTests, ExecuteTestBatteryCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("battery_comparator_test");

    execute_test_battery(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test enabler */
TEST(CommandsTests, ExecuteTestEnablerCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("power_enabler_test");

    execute_test_enabler(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test led */
TEST(CommandsTests, ExecuteTestLedCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("led_test");

    execute_test_led(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test pushbutton */
TEST(CommandsTests, ExecuteTestPushbuttonCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("pushbutton_test");

    execute_test_pushbutton(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test ir */
TEST(CommandsTests, TestIrCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";

    struct command_match match{
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, IrCommandContainsThreeSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(3u, node->child_count);
}

TEST(CommandsTests, FindCommandNodeReturnsIRNodes)
{
    for (auto const &test : ir_cases) {
        struct command cmd{{0}};
        cmd.token_count = 3;
        cmd.tokens[0] = "test";
        cmd.tokens[1] = test.parent;
        cmd.tokens[2] = test.child;

        struct command_node const *node =
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

        CHECK(node != nullptr);

        SimpleString msg =
            StringFromFormat("IR node mismatch parent=%s child=%s", test.parent, test.child);

        STRCMP_EQUAL_TEXT(test.child, node->name, msg.asCharString());
    }
}

TEST(CommandsTests, ValidateIRCommandsReturnSuccessAndTooMany)
{
    for (auto const &test : ir_cases) {
        struct command cmd{{0}};
        cmd.token_count = test.success_tokens;

        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_SUCCESS, test.validate(&cmd), test.child);
        cmd.token_count = test.too_many_tokens;
        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, test.validate(&cmd), test.child);
    }
}

/*----------------------------------------------------------------------------*/
/* test ir distance */
TEST(CommandsTests, ValidateTestIrDistanceReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_distance(&cmd));
}

TEST(CommandsTests, ExecuteTestIrDistanceCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 8;
    cmd.tokens[3] = "5";
    cmd.tokens[4] = "30";
    cmd.tokens[5] = "10";
    cmd.tokens[6] = "100";
    cmd.tokens[7] = "500";

    mock().expectOneCall("infrared_sensors_distance_test")
        .withUnsignedIntParameter("start_distance_cm", 5u)
        .withUnsignedIntParameter("end_distance_cm", 30u)
        .withUnsignedIntParameter("trials_per_distance", 10u)
        .withUnsignedIntParameter("time_per_trial_ms", 100u)
        .withUnsignedIntParameter("setup_delay_ms", 500u);

    execute_test_ir_distance(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test ir free */
TEST(CommandsTests, ValidateTestIrFreeReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_free(&cmd));
}

TEST(CommandsTests, ExecuteTestIrFreeCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 5;
    cmd.tokens[3] = "100";
    cmd.tokens[4] = "500";

    mock().expectOneCall("infrared_sensors_free_reading_test")
        .withUnsignedIntParameter("time_per_sensor_ms", 100u)
        .withUnsignedIntParameter("setup_delay_ms", 500u);

    execute_test_ir_free(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test ir speed */
TEST(CommandsTests, ValidateTestIrSpeedReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_speed(&cmd));
}

TEST(CommandsTests, ExecuteTestIrSpeedCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 4;
    cmd.tokens[3] = "250";

    mock().expectOneCall("infrared_sensors_read_speed_test")
        .withUnsignedIntParameter("time_per_sensor_ms", 250u);

    execute_test_ir_speed(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder */
TEST(CommandsTests, TestWheelEncoderCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";

    struct command_match match{
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, WheelEncoderCommandContainsSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(3u, node->child_count);
}

TEST(CommandsTests, FindCommandNodeReturnsWheelEncoderNodes)
{
    for (auto const &test : wheel_encoder_cases) {
        struct command cmd{{0}};
        cmd.token_count = 3;
        cmd.tokens[0] = "test";
        cmd.tokens[1] = test.parent;
        cmd.tokens[2] = test.child;

        struct command_node const *node =
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

        CHECK(node != nullptr);

        SimpleString msg = StringFromFormat("wheel-encoder node mismatch child=%s", test.child);

        STRCMP_EQUAL_TEXT(test.child, node->name, msg.asCharString());
    }
}

TEST(CommandsTests, ValidateWheelEncoderCommandsReturnSuccessAndTooMany)
{
    for (auto const &test : wheel_encoder_cases) {
        struct command cmd{{0}};
        cmd.token_count = test.success_tokens;

        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_SUCCESS, test.validate(&cmd), test.child);

        cmd.token_count = test.too_many_tokens;

        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, test.validate(&cmd), test.child);
    }
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder-target */
TEST(CommandsTests, ValidateTestWheelEncoderTargetReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_wheel_encoder_target(&cmd));
}

TEST(CommandsTests, ExecuteTestWheelEncoderTargetCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 9;
    cmd.tokens[3] = "5000";
    cmd.tokens[4] = "100";
    cmd.tokens[5] = "2500";
    cmd.tokens[6] = "10";
    cmd.tokens[7] = "50";
    cmd.tokens[8] = "5";

    mock().expectOneCall("wheel_motor_and_encoder_test")
        .withUnsignedIntParameter("timeout_ms", 5000u)
        .withUnsignedIntParameter("drift_delay_ms", 100u)
        .withIntParameter("encoder_target", 2500)
        .withUnsignedIntParameter("start_speed", 10u)
        .withUnsignedIntParameter("end_speed", 50u)
        .withUnsignedIntParameter("speed_step", 5u);

    execute_test_wheel_encoder_target(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder-drift */
TEST(CommandsTests, ValidateTestWheelEncoderDriftReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS,
                validate_test_wheel_encoder_drift(&cmd));
}

TEST(CommandsTests, ExecuteTestWheelEncoderDriftCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 9;
    cmd.tokens[3] = "5000";
    cmd.tokens[4] = "100";
    cmd.tokens[5] = "2500";
    cmd.tokens[6] = "10";
    cmd.tokens[7] = "50";
    cmd.tokens[8] = "5";

    mock().expectOneCall("wheel_motor_drift_test")
        .withUnsignedIntParameter("timeout_ms", 5000u)
        .withUnsignedIntParameter("drift_delay_ms", 100u)
        .withIntParameter("encoder_target", 2500)
        .withUnsignedIntParameter("start_speed", 10u)
        .withUnsignedIntParameter("end_speed", 50u)
        .withUnsignedIntParameter("speed_step", 5u);

    execute_test_wheel_encoder_drift(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder-deceleration */
TEST(CommandsTests, ValidateTestWheelEncoderDecelerationReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS,
                validate_test_wheel_encoder_deceleration(&cmd));
}

TEST(CommandsTests, ExecuteTestWheelEncoderDecelerationCallsFunctions)
{
    struct command cmd{{0}};
    cmd.token_count = 9;
    cmd.tokens[3] = "5000";
    cmd.tokens[4] = "100";
    cmd.tokens[5] = "2500";
    cmd.tokens[6] = "10";
    cmd.tokens[7] = "80";
    cmd.tokens[8] = "25";

    mock().expectOneCall("wheel_motor_deceleration_test")
        .withUnsignedIntParameter("timeout_ms", 5000u)
        .withUnsignedIntParameter("drift_delay_ms", 100u)
        .withIntParameter("encoder_target", 2500)
        .withUnsignedIntParameter("start_speed", 10u)
        .withUnsignedIntParameter("top_speed", 80u)
        .withUnsignedIntParameter("max_accel_decel_percent", 25u);

    execute_test_wheel_encoder_deceleration(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test vacuum */
TEST(CommandsTests, ExecuteTestVacuumCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("vacuum_test");

    execute_test_vacuum(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate */
TEST(CommandsTests, NavigateCommandContainsSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "navigate";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(8u, node->child_count);
}

TEST(CommandsTests, NavigateCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_test_commands()[8].children;

    STRCMP_EQUAL("move-forward", commands[0].name);
    STRCMP_EQUAL("move-forward-continuous", commands[1].name);
    STRCMP_EQUAL("rotate-clockwise-90", commands[2].name);
    STRCMP_EQUAL("rotate-counterclockwise-90", commands[3].name);
    STRCMP_EQUAL("rotate-180", commands[4].name);
    STRCMP_EQUAL("left-wall-presence", commands[5].name);
    STRCMP_EQUAL("right-wall-presence", commands[6].name);
    STRCMP_EQUAL("front-wall-presence", commands[7].name);
}

TEST(CommandsTests, TestNavigateCommandMatchDepthIsTwo)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "navigate";

    struct command_match match{
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT)};

    LONGS_EQUAL(2u, match.depth);
}

TEST(CommandsTests, FindCommandNodeReturnsNavigateNodes)
{
    for (auto const &test : navigate_cases) {
        struct command cmd{{0}};
        cmd.token_count = 3;
        cmd.tokens[0] = "test";
        cmd.tokens[1] = test.parent;
        cmd.tokens[2] = test.child;

        struct command_node const *node =
            find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

        CHECK(node != nullptr);

        SimpleString msg = StringFromFormat("navigate node mismatch child=%s", test.child);
        STRCMP_EQUAL_TEXT(test.child, node->name, msg.asCharString());
    }
}

TEST(CommandsTests, ValidateNavigateCommandsReturnSuccessAndTooMany)
{
    for (auto const &test : navigate_cases) {
        struct command cmd{{0}};
        cmd.token_count = test.success_tokens;
        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_SUCCESS, test.validate(&cmd), test.child);
        cmd.token_count = test.too_many_tokens;
        LONGS_EQUAL_TEXT(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, test.validate(&cmd), test.child);
    }
}

/*----------------------------------------------------------------------------*/
/* test navigate move forward */
TEST(CommandsTests, ExecuteTestNavigateMoveForwardCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("move_forward");
    mock().expectOneCall("get_move_forward_statistics");

    execute_test_navigate_move_forward(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate move forward continuous */
TEST(CommandsTests, ExecuteTestNavigateMoveForwardContinuousCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("move_forward_until_turn_or_intersection_and_return_steps");
    mock().expectOneCall("get_move_forward_statistics");

    execute_test_navigate_move_forward_continuous(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate clockwise 90 */
TEST(CommandsTests, ExecuteTestNavigateRotateClockwise90CallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("rotate_clockwise_90_deg");
    mock().expectOneCall("get_rotate_statistics");

    execute_test_navigate_rotate_clockwise_90(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate counterclockwise 90 */
TEST(CommandsTests, ExecuteTestNavigateRotateCounterClockwise90CallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("rotate_counter_clockwise_90_deg");
    mock().expectOneCall("get_rotate_statistics");

    execute_test_navigate_rotate_counterclockwise_90(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate rotate 180 */
TEST(CommandsTests, ExecuteTestNavigateRotate180CallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("rotate_180_deg");
    mock().expectOneCall("get_rotate_statistics");

    execute_test_navigate_rotate_180(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate left-wall presence */
TEST(CommandsTests, ExecuteTestNavigateLeftWallPresenceCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("is_left_wall_present");

    execute_test_navigate_left_wall_presence(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate right-wall presence*/
TEST(CommandsTests, ExecuteTestNavigateRightWallPresenceCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("is_right_wall_present");

    execute_test_navigate_right_wall_presence(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test navigate front-wall presence */
TEST(CommandsTests, ExecuteTestNavigateFrontWallPresenceCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("is_front_wall_present");

    execute_test_navigate_front_wall_presence(&cmd);
}
