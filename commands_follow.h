#ifndef _COMMANDS_FOLLOW_H_
#define _COMMANDS_FOLLOW_H_

#include "helpers.h"
#include "commands.h"
#include "parson.h"

int extract_login_cookie(char *response);
int extract_jwt_token(char *response);
int print_JSON(char *response, int mode);

#endif