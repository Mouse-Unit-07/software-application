/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : solve.h                                               */
/*                                                                            */
/* Interface for micromouse solve library                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef SOLVE_H_
#define SOLVE_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
const struct command_node *get_solve_node(void);
const struct command_node *get_solve_commands(void);
uint32_t get_solve_commands_count(void);

enum validation_result validate_solve(struct command *cmd);
void execute_solve(struct command const *cmd);

enum validation_result validate_solve_wallfollower(struct command *cmd);
void execute_solve_wallfollower(struct command const *cmd);

#endif /* SOLVE_H_ */
