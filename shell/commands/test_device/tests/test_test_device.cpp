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
    fake_root_commands[0].execute = NULL;
    fake_root_commands[0].children = get_test_commands();
    fake_root_commands[0].child_count = get_test_commands_count();
}

void deinit_test_fakes(void)
{
    fake_root_commands[0] = {};
}

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
    CHECK(node->validate != nullptr);
    POINTERS_EQUAL(get_test_commands(), node->children);
    LONGS_EQUAL(get_test_commands_count(), node->child_count);
}

TEST(CommandsTests, GetTestCommandsCountReturnsExpectedValue)
{
    LONGS_EQUAL(7u, get_test_commands_count());
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
}

TEST(CommandsTests, TestCommandContainsSevenSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(7u, node->child_count);
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
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test(&cmd));
}

TEST(CommandsTests, ValidateTestReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 1;
    cmd.tokens[0] = "test";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test(&cmd));
}

/*----------------------------------------------------------------------------*/
/* test processor */
TEST(CommandsTests, FindCommandNodeReturnsProcessorNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("processor", node->name);
}

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

TEST(CommandsTests, ValidateTestProcessorReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_processor(&cmd));
}

TEST(CommandsTests, ValidateTestProcessorReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "processor";
    cmd.tokens[2] = "extra";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_processor(&cmd));
}

TEST(CommandsTests, ExecuteTestProcessorCallsFunctions)
{
    struct command cmd{{0}};
    mock().expectOneCall("processor_test");

    execute_test_processor(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test battery */
TEST(CommandsTests, FindCommandNodeReturnsBatteryNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "battery";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("battery", node->name);
}

TEST(CommandsTests, ValidateTestBatteryReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "battery";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_battery(&cmd));
}

TEST(CommandsTests, ExecuteTestBatteryCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("battery_comparator_test");

    execute_test_battery(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test enabler */
TEST(CommandsTests, FindCommandNodeReturnsEnablerNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "enabler";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("enabler", node->name);
}

TEST(CommandsTests, ValidateTestEnablerReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "enabler";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_enabler(&cmd));
}

TEST(CommandsTests, ExecuteTestEnablerCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("power_enabler_test");

    execute_test_enabler(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test led */
TEST(CommandsTests, FindCommandNodeReturnsLedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "led";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("led", node->name);
}

TEST(CommandsTests, ValidateTestLedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "led";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_led(&cmd));
}

TEST(CommandsTests, ExecuteTestLedCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("led_test");

    execute_test_led(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test pushbutton */
TEST(CommandsTests, FindCommandNodeReturnsPushbuttonNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "pushbutton";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("pushbutton", node->name);
}

TEST(CommandsTests, ValidateTestPushbuttonReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "pushbutton";

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_pushbutton(&cmd));
}

TEST(CommandsTests, ExecuteTestPushbuttonCallsFunctions)
{
    struct command cmd{{0}};

    mock().expectOneCall("pushbutton_test");

    execute_test_pushbutton(&cmd);
}

/*----------------------------------------------------------------------------*/
/* test ir */
TEST(CommandsTests, FindCommandNodeReturnsIrNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("ir", node->name);
}

TEST(CommandsTests, ValidateTestIrReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir(&cmd));
}

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

TEST(CommandsTests, IrCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_test_commands()[5].children;

    STRCMP_EQUAL("distance", commands[0].name);
    STRCMP_EQUAL("free", commands[1].name);
    STRCMP_EQUAL("speed", commands[2].name);
}

/*----------------------------------------------------------------------------*/
/* test ir distance */
TEST(CommandsTests, FindCommandNodeReturnsIrDistanceNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";
    cmd.tokens[2] = "distance";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("distance", node->name);
}

TEST(CommandsTests, ValidateTestIrDistanceReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 8;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_ir_distance(&cmd));
}

TEST(CommandsTests, ValidateTestIrDistanceReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_distance(&cmd));
}

TEST(CommandsTests, ValidateTestIrDistanceReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 9;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_ir_distance(&cmd));
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
TEST(CommandsTests, FindCommandNodeReturnsIrFreeNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";
    cmd.tokens[2] = "free";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("free", node->name);
}

TEST(CommandsTests, ValidateTestIrFreeReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 5;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_ir_free(&cmd));
}

TEST(CommandsTests, ValidateTestIrFreeReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_free(&cmd));
}

TEST(CommandsTests, ValidateTestIrFreeReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 6;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_ir_free(&cmd));
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
TEST(CommandsTests, FindCommandNodeReturnsIrSpeedNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "ir";
    cmd.tokens[2] = "speed";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("speed", node->name);
}

TEST(CommandsTests, ValidateTestIrSpeedReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 4;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_ir_speed(&cmd));
}

TEST(CommandsTests, ValidateTestIrSpeedReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_ir_speed(&cmd));
}

TEST(CommandsTests, ValidateTestIrSpeedReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 5;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_ir_speed(&cmd));
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
TEST(CommandsTests, FindCommandNodeReturnsWheelEncoderNode)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("wheel-encoder", node->name);
}

TEST(CommandsTests, ValidateTestWheelEncoderReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_wheel_encoder(&cmd));
}

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

TEST(CommandsTests, WheelEncoderCommandContainsTwoSubcommands)
{
    struct command cmd{{0}};
    cmd.token_count = 2;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    LONGS_EQUAL(2u, node->child_count);
}

TEST(CommandsTests, WheelEncoderCommandsAreInExpectedOrder)
{
    const struct command_node *commands = get_test_commands()[6].children;

    STRCMP_EQUAL("target", commands[0].name);
    STRCMP_EQUAL("deceleration", commands[1].name);
}

/*----------------------------------------------------------------------------*/
/* test wheel-encoder-target */
TEST(CommandsTests, FindCommandNodeReturnsWheelEncoderTargetNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;
    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";
    cmd.tokens[2] = "target";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("target", node->name);
}

TEST(CommandsTests, ValidateTestWheelEncoderTargetReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 9;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_wheel_encoder_target(&cmd));
}

TEST(CommandsTests, ValidateTestWheelEncoderTargetReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS, validate_test_wheel_encoder_target(&cmd));
}

TEST(CommandsTests, ValidateTestWheelEncoderTargetReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 10;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS, validate_test_wheel_encoder_target(&cmd));
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
/* test wheel-encoder-deceleration */
TEST(CommandsTests, FindCommandNodeReturnsWheelEncoderDecelerationNode)
{
    struct command cmd{{0}};
    cmd.token_count = 3;

    cmd.tokens[0] = "test";
    cmd.tokens[1] = "wheel-encoder";
    cmd.tokens[2] = "deceleration";

    struct command_node const *node =
        find_command_node(&cmd, fake_root_commands, FAKE_ROOT_COMMANDS_COUNT).node;

    CHECK(node != nullptr);
    STRCMP_EQUAL("deceleration", node->name);
}

TEST(CommandsTests, ValidateTestWheelEncoderDecelerationReturnsSuccess)
{
    struct command cmd{{0}};
    cmd.token_count = 9;

    LONGS_EQUAL(COMMAND_VALIDATION_SUCCESS, validate_test_wheel_encoder_deceleration(&cmd));
}

TEST(CommandsTests, ValidateTestWheelEncoderDecelerationReturnsTooFewParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 7;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_FEW_PARAMETERS,
                validate_test_wheel_encoder_deceleration(&cmd));
}

TEST(CommandsTests, ValidateTestWheelEncoderDecelerationReturnsTooManyParameters)
{
    struct command cmd{{0}};
    cmd.token_count = 10;

    LONGS_EQUAL(COMMAND_VALIDATION_TOO_MANY_PARAMETERS,
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
