/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : configuration.h                                       */
/*                                                                            */
/* Interface for micromouse configuration library                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
void init_configuration(void);
void deinit_configuration(void);

struct maze_solver_config get_default_maze_solver_config(void);
struct maze_solver_config get_test_maze_solver_config(void);
void set_test_maze_solver_config(struct maze_solver_config cfg);

#endif /* CONFIGURATION_H_ */
