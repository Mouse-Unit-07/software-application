/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : set.h                                                 */
/*                                                                            */
/* Interface for micromouse set library                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef SET_H_
#define SET_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
const struct command_node *get_set_node(void);
const struct command_node *get_set_commands(void);
uint32_t get_set_commands_count(void);

enum validation_result validate_set(struct command *cmd);

enum validation_result validate_set_solver_default(struct command *cmd);
void execute_set_solver_default(struct command const *cmd);

enum validation_result validate_set_solver_test(struct command *cmd);
void execute_set_solver_test(struct command const *cmd);

#endif /* SET_H_ */
