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

struct move_forward_control_config get_saved_default_move_forward_control_no_wall_config(void);
struct move_forward_control_config get_saved_test_move_forward_control_no_wall_config(void);
void save_move_forward_control_no_wall_config_as_test(struct move_forward_control_config cfg);

struct move_forward_control_config get_saved_default_move_forward_control_one_wall_config(void);
struct move_forward_control_config get_saved_test_move_forward_control_one_wall_config(void);
void save_move_forward_control_one_wall_config_as_test(struct move_forward_control_config cfg);

struct move_forward_control_config get_saved_default_move_forward_control_both_wall_config(void);
struct move_forward_control_config get_saved_test_move_forward_control_both_wall_config(void);
void save_move_forward_control_both_wall_config_as_test(struct move_forward_control_config cfg);

struct rotate_control_config get_saved_default_rotate_control_config(void);
struct rotate_control_config get_saved_test_rotate_control_config(void);
void save_rotate_control_config_as_test(struct rotate_control_config cfg);

struct front_wall_detection_config get_saved_default_front_wall_detection_config(void);
struct front_wall_detection_config get_saved_test_front_wall_detection_config(void);
void save_front_wall_detection_config_as_test(struct front_wall_detection_config cfg);

struct side_wall_detection_config get_saved_default_side_wall_detection_config(void);
struct side_wall_detection_config get_saved_test_side_wall_detection_config(void);
void save_side_wall_detection_config_as_test(struct side_wall_detection_config cfg);

#endif /* CONFIGURATION_H_ */
