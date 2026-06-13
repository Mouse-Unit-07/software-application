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
void execute_set(struct command const *cmd);

/* maze solver */
enum validation_result validate_set_solver_default(struct command *cmd);
void execute_set_solver_default(struct command const *cmd);

enum validation_result validate_set_solver_test(struct command *cmd);
void execute_set_solver_test(struct command const *cmd);

/* mouse */
enum validation_result validate_set_mouse_physical_default(struct command *cmd);
void execute_set_mouse_physical_default(struct command const *cmd);

enum validation_result validate_set_mouse_physical_test(struct command *cmd);
void execute_set_mouse_physical_test(struct command const *cmd);

/* maze */
enum validation_result validate_set_maze_physical_default(struct command *cmd);
void execute_set_maze_physical_default(struct command const *cmd);

enum validation_result validate_set_maze_physical_test(struct command *cmd);
void execute_set_maze_physical_test(struct command const *cmd);

/* move forward */
enum validation_result validate_set_move_forward_no_wall_default(struct command *cmd);
void execute_set_move_forward_no_wall_default(struct command const *cmd);

enum validation_result validate_set_move_forward_no_wall_test(struct command *cmd);
void execute_set_move_forward_no_wall_test(struct command const *cmd);

enum validation_result validate_set_move_forward_one_wall_default(struct command *cmd);
void execute_set_move_forward_one_wall_default(struct command const *cmd);

enum validation_result validate_set_move_forward_one_wall_test(struct command *cmd);
void execute_set_move_forward_one_wall_test(struct command const *cmd);

enum validation_result validate_set_move_forward_both_wall_default(struct command *cmd);
void execute_set_move_forward_both_wall_default(struct command const *cmd);

enum validation_result validate_set_move_forward_both_wall_test(struct command *cmd);
void execute_set_move_forward_both_wall_test(struct command const *cmd);

/* rotate */
enum validation_result validate_set_rotate_default(struct command *cmd);
void execute_set_rotate_default(struct command const *cmd);

enum validation_result validate_set_rotate_test(struct command *cmd);
void execute_set_rotate_test(struct command const *cmd);

/* front wall */
enum validation_result validate_set_front_wall_default(struct command *cmd);
void execute_set_front_wall_default(struct command const *cmd);

enum validation_result validate_set_front_wall_test(struct command *cmd);
void execute_set_front_wall_test(struct command const *cmd);

/* side wall */
enum validation_result validate_set_side_wall_default(struct command *cmd);
void execute_set_side_wall_default(struct command const *cmd);

enum validation_result validate_set_side_wall_test(struct command *cmd);
void execute_set_side_wall_test(struct command const *cmd);

#endif /* SET_H_ */
