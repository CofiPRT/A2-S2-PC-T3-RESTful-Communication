#ifndef _COMMANDS_HELPERS_H_
#define _COMMANDS_HELPERS_H_

#include "helpers.h"
#include "commands.h"
#include "parson.h"
#include "requests.h"

#define MAX_ATTEMPTS		100

int get_error(char *response, char *error_msg);
int check_code(char *response, char *code);
int attempt_send(char *message, char **response);

#endif