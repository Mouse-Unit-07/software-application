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
void execute_get(struct command const *cmd);

enum validation_result validate_get_solver_default(struct command *cmd);
void execute_get_solver_default(struct command const *cmd);

enum validation_result validate_get_solver_test(struct command *cmd);
void execute_get_solver_test(struct command const *cmd);

enum validation_result validate_get_solver_current(struct command *cmd);
void execute_get_solver_current(struct command const *cmd);

enum validation_result validate_get_mouse_physical_default(struct command *cmd);
void execute_get_mouse_physical_default(struct command const *cmd);

enum validation_result validate_get_mouse_physical_test(struct command *cmd);
void execute_get_mouse_physical_test(struct command const *cmd);

enum validation_result validate_get_mouse_physical_current(struct command *cmd);
void execute_get_mouse_physical_current(struct command const *cmd);

enum validation_result validate_get_mouse_calculated(struct command *cmd);
void execute_get_mouse_calculated(struct command const *cmd);

enum validation_result validate_get_maze_physical_default(struct command *cmd);
void execute_get_maze_physical_default(struct command const *cmd);

enum validation_result validate_get_maze_physical_test(struct command *cmd);
void execute_get_maze_physical_test(struct command const *cmd);

enum validation_result validate_get_maze_physical_current(struct command *cmd);
void execute_get_maze_physical_current(struct command const *cmd);

enum validation_result validate_get_maze_calculated(struct command *cmd);
void execute_get_maze_calculated(struct command const *cmd);

enum validation_result validate_get_navigation(struct command *cmd);
void execute_get_navigation(struct command const *cmd);

enum validation_result validate_get_move_forward_common_default(struct command *cmd);
void execute_get_move_forward_common_default(struct command const *cmd);

enum validation_result validate_get_move_forward_common_test(struct command *cmd);
void execute_get_move_forward_common_test(struct command const *cmd);

enum validation_result validate_get_move_forward_common_current(struct command *cmd);
void execute_get_move_forward_common_current(struct command const *cmd);

enum validation_result validate_get_move_forward_no_wall_default(struct command *cmd);
void execute_get_move_forward_no_wall_default(struct command const *cmd);

enum validation_result validate_get_move_forward_no_wall_test(struct command *cmd);
void execute_get_move_forward_no_wall_test(struct command const *cmd);

enum validation_result validate_get_move_forward_no_wall_current(struct command *cmd);
void execute_get_move_forward_no_wall_current(struct command const *cmd);

enum validation_result validate_get_move_forward_no_wall_calculated(struct command *cmd);
void execute_get_move_forward_no_wall_calculated(struct command const *cmd);

enum validation_result validate_get_move_forward_one_wall_default(struct command *cmd);
void execute_get_move_forward_one_wall_default(struct command const *cmd);

enum validation_result validate_get_move_forward_one_wall_test(struct command *cmd);
void execute_get_move_forward_one_wall_test(struct command const *cmd);

enum validation_result validate_get_move_forward_one_wall_current(struct command *cmd);
void execute_get_move_forward_one_wall_current(struct command const *cmd);

enum validation_result validate_get_move_forward_one_wall_calculated(struct command *cmd);
void execute_get_move_forward_one_wall_calculated(struct command const *cmd);

enum validation_result validate_get_move_forward_both_wall_default(struct command *cmd);
void execute_get_move_forward_both_wall_default(struct command const *cmd);

enum validation_result validate_get_move_forward_both_wall_test(struct command *cmd);
void execute_get_move_forward_both_wall_test(struct command const *cmd);

enum validation_result validate_get_move_forward_both_wall_current(struct command *cmd);
void execute_get_move_forward_both_wall_current(struct command const *cmd);

enum validation_result validate_get_move_forward_both_wall_calculated(struct command *cmd);
void execute_get_move_forward_both_wall_calculated(struct command const *cmd);

enum validation_result validate_get_rotate_default(struct command *cmd);
void execute_get_rotate_default(struct command const *cmd);

enum validation_result validate_get_rotate_test(struct command *cmd);
void execute_get_rotate_test(struct command const *cmd);

enum validation_result validate_get_rotate_current(struct command *cmd);
void execute_get_rotate_current(struct command const *cmd);

enum validation_result validate_get_front_wall_default(struct command *cmd);
void execute_get_front_wall_default(struct command const *cmd);

enum validation_result validate_get_front_wall_test(struct command *cmd);
void execute_get_front_wall_test(struct command const *cmd);

enum validation_result validate_get_front_wall_current(struct command *cmd);
void execute_get_front_wall_current(struct command const *cmd);

enum validation_result validate_get_side_wall_default(struct command *cmd);
void execute_get_side_wall_default(struct command const *cmd);

enum validation_result validate_get_side_wall_test(struct command *cmd);
void execute_get_side_wall_test(struct command const *cmd);

enum validation_result validate_get_side_wall_current(struct command *cmd);
void execute_get_side_wall_current(struct command const *cmd);

enum validation_result validate_get_side_wall_calculated(struct command *cmd);
void execute_get_side_wall_calculated(struct command const *cmd);

#endif /* GET_H_ */
