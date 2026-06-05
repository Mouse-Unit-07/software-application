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
    MAX_COMMAND_SIZE = 32,
    MAX_PARAMETER_COUNT = 8
};

struct command {
    char command[MAX_COMMAND_SIZE];
    uint32_t parameter_count;
    char const *parameters[MAX_PARAMETER_COUNT];
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

struct command_table_entry {
    char const *name;
    char const *help;
    enum validation_result (*validate)(struct command const *cmd);
    void (*execute)(struct command const *cmd);
};

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
uint32_t get_command_table_size(void);
struct command_table_entry get_command_table_entry_at_index(uint32_t index);

enum validation_result validate_help(struct command const *cmd);
void execute_help(struct command const *cmd);

enum validation_result validate_clear(struct command const *cmd);
void execute_clear(struct command const *cmd);

enum validation_result validate_hardware_faults(struct command const *cmd);
void execute_hardware_faults(struct command const *cmd);

#endif /* COMMANDS_H_ */
