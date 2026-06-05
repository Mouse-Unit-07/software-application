/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : shell.h                                               */
/*                                                                            */
/* Interface for micromouse shell library                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef SHELL_H_
#define SHELL_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* definitions exposed for testing */
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

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
void init_shell(void);
void deinit_shell(void);
void poll_shell(void);

/* helpers exposed for testing */
void load_cli_buffer_contents(void);
struct command parse_cli_buffer_contents(void);
void process_command(struct command const *cmd);

enum validation_result validate_help(struct command const *cmd);
void execute_help(struct command const *cmd);
enum validation_result validate_clear(struct command const *cmd);
void execute_clear(struct command const *cmd);

char *get_shell_buffer(void);
uint32_t get_shell_buffer_size(void);
bool get_ready_for_parsing(void);

#endif /* SHELL_H_ */
