/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : commands.h                                            */
/*                                                                            */
/* Interface for micromouse commands library                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef COMMANDS_H_
#define COMMANDS_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
enum
{
    MAX_COMMAND_NAME_SIZE = 32,
    MAX_PARAMETER_COUNT = 8
};

struct command {
    uint32_t token_count;
    char const *tokens[MAX_PARAMETER_COUNT + 1];

    uint32_t bad_parameter_index;
};

enum validation_result
{
    COMMAND_VALIDATION_SUCCESS,
    COMMAND_VALIDATION_BAD_PARAMETER,
    COMMAND_VALIDATION_TOO_MANY_PARAMETERS,
    COMMAND_VALIDATION_TOO_FEW_PARAMETERS,
    COMMAND_VALIDATION_NOT_MATCHED
};

struct command_node {
    char const *name;
    char const *help;

    enum validation_result (*validate)(struct command const *cmd);
    void (*execute)(struct command const *cmd);

    struct command_node const *children;
    uint32_t child_count;
};

struct command_match {
    struct command_node const *node;
    uint32_t depth;
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
struct command_node const *get_command_tree_root(void);
struct command_match find_command_node(struct command const *cmd);
uint32_t get_command_tree_root_count(void);

enum validation_result validate_help(struct command const *cmd);
void execute_help(struct command const *cmd);

enum validation_result validate_clear(struct command const *cmd);
void execute_clear(struct command const *cmd);

enum validation_result validate_hardware_faults(struct command const *cmd);
void execute_hardware_faults(struct command const *cmd);

enum validation_result validate_get_time(struct command const *cmd);
void execute_get_time(struct command const *cmd);

enum validation_result validate_test(struct command const *cmd);

enum validation_result validate_test_processor(struct command const *cmd);
void execute_test_processor(struct command const *cmd);

enum validation_result validate_test_battery(struct command const *cmd);
void execute_test_battery(struct command const *cmd);

enum validation_result validate_test_enabler(struct command const *cmd);
void execute_test_enabler(struct command const *cmd);

enum validation_result validate_test_led(struct command const *cmd);
void execute_test_led(struct command const *cmd);

enum validation_result validate_test_pushbutton(struct command const *cmd);
void execute_test_pushbutton(struct command const *cmd);

#endif /* COMMANDS_H_ */
