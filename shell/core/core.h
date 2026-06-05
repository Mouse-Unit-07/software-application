/*-------------------------------- FILE INFO ---------------------------------*/
/* Filename           : core.h                                               */
/*                                                                            */
/* Interface for micromouse core library                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#ifndef CORE_H_
#define CORE_H_

/*----------------------------------------------------------------------------*/
/*                             Public Definitions                             */
/*----------------------------------------------------------------------------*/
/* none */

/*----------------------------------------------------------------------------*/
/*                         Public Function Prototypes                         */
/*----------------------------------------------------------------------------*/
void init_shell(void);
void deinit_shell(void);
void poll_shell(void);

/* helpers exposed for testing */
void load_cli_buffer_contents(void);
struct command parse_cli_buffer_contents(void);
void process_command(struct command const *cmd);

char *get_shell_buffer(void);
uint32_t get_shell_buffer_size(void);
bool get_ready_for_parsing(void);

#endif /* CORE_H_ */
