/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : test_device.h                                         */
/*                                                                            */
/* Interface for micromouse test_device library                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef TEST_DEVICE_H_
#define TEST_DEVICE_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
const struct command_node *get_test_node(void);
const struct command_node *get_test_commands(void);
uint32_t get_test_commands_count(void);

enum validation_result validate_test(struct command *cmd);

enum validation_result validate_test_processor(struct command *cmd);
void execute_test_processor(struct command const *cmd);

enum validation_result validate_test_battery(struct command *cmd);
void execute_test_battery(struct command const *cmd);

enum validation_result validate_test_enabler(struct command *cmd);
void execute_test_enabler(struct command const *cmd);

enum validation_result validate_test_led(struct command *cmd);
void execute_test_led(struct command const *cmd);

enum validation_result validate_test_pushbutton(struct command *cmd);
void execute_test_pushbutton(struct command const *cmd);

enum validation_result validate_test_ir(struct command *cmd);

enum validation_result validate_test_ir_distance(struct command *cmd);
void execute_test_ir_distance(struct command const *cmd);

enum validation_result validate_test_ir_free(struct command *cmd);
void execute_test_ir_free(struct command const *cmd);

enum validation_result validate_test_ir_speed(struct command *cmd);
void execute_test_ir_speed(struct command const *cmd);

enum validation_result validate_test_wheel_encoder(struct command *cmd);

enum validation_result validate_test_wheel_encoder_target(struct command *cmd);
void execute_test_wheel_encoder_target(struct command const *cmd);

enum validation_result validate_test_wheel_encoder_deceleration(struct command *cmd);
void execute_test_wheel_encoder_deceleration(struct command const *cmd);

enum validation_result validate_test_vacuum(struct command *cmd);
void execute_test_vacuum(struct command const *cmd);

/* navigation tests */
enum validation_result validate_test_navigate(struct command *cmd);

enum validation_result validate_test_navigate_move_forward(struct command *cmd);
void execute_test_navigate_move_forward(struct command const *cmd);

enum validation_result validate_test_navigate_rotate_clockwise_90(struct command *cmd);
void execute_test_navigate_rotate_clockwise_90(struct command const *cmd);

enum validation_result validate_test_navigate_rotate_counterclockwise_90(struct command *cmd);
void execute_test_navigate_rotate_counterclockwise_90(struct command const *cmd);

enum validation_result validate_test_navigate_rotate_180(struct command *cmd);
void execute_test_navigate_rotate_180(struct command const *cmd);

enum validation_result validate_test_navigate_left_wall_presence(struct command *cmd);
void execute_test_navigate_left_wall_presence(struct command const *cmd);

enum validation_result validate_test_navigate_right_wall_presence(struct command *cmd);
void execute_test_navigate_right_wall_presence(struct command const *cmd);

enum validation_result validate_test_navigate_front_wall_presence(struct command *cmd);
void execute_test_navigate_front_wall_presence(struct command const *cmd);

#endif /* TEST_DEVICE_H_ */
