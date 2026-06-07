/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : get.h                                                 */
/*                                                                            */
/* Interface for micromouse get library                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef GET_H_
#define GET_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
const struct command_node *get_get_node(void);
const struct command_node *get_get_commands(void);
uint32_t get_get_commands_count(void);

enum validation_result validate_get(struct command *cmd);

enum validation_result validate_get_solver_default(struct command *cmd);
void execute_get_solver_default(struct command const *cmd);

enum validation_result validate_get_solver_test(struct command *cmd);
void execute_get_solver_test(struct command const *cmd);

enum validation_result validate_get_solver_current(struct command *cmd);
void execute_get_solver_current(struct command const *cmd);

#endif /* GET_H_ */
