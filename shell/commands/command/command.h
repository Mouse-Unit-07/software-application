/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : command.h                                             */
/*                                                                            */
/* Interface for micromouse command library                                   */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef COMMAND_H_
#define COMMAND_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
enum
{
    MAX_COMMAND_NAME_SIZE = 32,
    MAX_PARAMETER_COUNT = 16
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
    enum validation_result (*validate)(struct command *cmd);
    void (*execute)(struct command const *cmd);
    struct command_node const *children;
    uint32_t child_count;
};

struct command_match {
    struct command_node const *node;
    uint32_t depth;
};

enum
{
    PARAM_0_OFFSET = 0,
    PARAM_1_OFFSET,
    PARAM_2_OFFSET,
    PARAM_3_OFFSET,
    PARAM_4_OFFSET,
    PARAM_5_OFFSET,
    PARAM_6_OFFSET,
    PARAM_7_OFFSET,
    PARAM_8_OFFSET,
    PARAM_9_OFFSET,
    PARAM_10_OFFSET
};

enum
{
    ROOT_COMMAND_TOKEN_COUNT = 1,
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
enum validation_result validate_parameterless_command(struct command const *cmd,
                                                      uint32_t token_count);

struct command_match find_command_node(struct command const *cmd,
                                       struct command_node const *commands, uint32_t command_count);

void print_command_help(struct command_node const *commands, uint32_t count);

#endif /* COMMAND_H_ */
