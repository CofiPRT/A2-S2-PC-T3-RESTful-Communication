#ifndef _COMMANDS_REQUESTS_H_
#define _COMMANDS_REQUESTS_H_

#include "helpers.h"
#include "client.h"
#include "parson.h"
#include "requests.h"
#include "commands.h"
#include "commands_helpers.h"

int userpass_post(char *path, char **response);
int simple_get(char *path, char **response);
int books_get(int mode, char **response);
int book_post(char **response);
int book_delete(char **response);

#endif