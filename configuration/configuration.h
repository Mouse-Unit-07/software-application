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

struct maze_solver_config get_saved_default_maze_solver_config(void);
struct maze_solver_config get_saved_test_maze_solver_config(void);
void save_maze_solver_config_as_test(struct maze_solver_config cfg);

struct mouse_physical_params get_saved_default_mouse_physical_params(void);
struct mouse_physical_params get_saved_test_mouse_physical_params(void);
void save_mouse_physical_params_as_test(struct mouse_physical_params cfg);

struct maze_physical_params get_saved_default_maze_physical_params(void);
struct maze_physical_params get_saved_test_maze_physical_params(void);
void save_maze_physical_params_as_test(struct maze_physical_params cfg);

#endif /* CONFIGURATION_H_ */
