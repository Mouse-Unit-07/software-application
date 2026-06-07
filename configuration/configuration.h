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

struct mouse_physical_params get_default_mouse_physical_params(void);
struct mouse_physical_params get_test_mouse_physical_params(void);
void set_test_mouse_physical_params(struct mouse_physical_params cfg);

struct maze_physical_params get_default_maze_physical_params(void);
struct maze_physical_params get_test_maze_physical_params(void);
void set_test_maze_physical_params(struct maze_physical_params cfg);

#endif /* CONFIGURATION_H_ */
