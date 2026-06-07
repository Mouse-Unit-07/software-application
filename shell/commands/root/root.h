/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : root.h                                                */
/*                                                                            */
/* Interface for micromouse root library                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef ROOT_H_
#define ROOT_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
void init_root(void);
void deinit_root(void);

struct command_node const *get_root_commands(void);
uint32_t get_root_commands_count(void);

struct command_match find_command_node_in_root(struct command const *cmd);

enum validation_result validate_help(struct command *cmd);
void execute_help(struct command const *cmd);

enum validation_result validate_clear(struct command *cmd);
void execute_clear(struct command const *cmd);

enum validation_result validate_hardware_faults(struct command *cmd);
void execute_hardware_faults(struct command const *cmd);

enum validation_result validate_time(struct command *cmd);
void execute_time(struct command const *cmd);

#endif /* ROOT_H_ */
