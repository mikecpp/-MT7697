#ifndef __CLI_H__
#define __CLI_H__

#include "Arduino.h"

typedef int (*cli_func)(int argc, char *argv[]);

int cli_init(void);
int cli_register(cli_func func, char *cmd, char *help); 
int cli_exec(char *string);
int cli_task(void);

#endif//__CLI_H__
