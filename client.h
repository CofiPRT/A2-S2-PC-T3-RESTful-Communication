#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "helpers.h"
#include "commands.h"

extern char login_cookie[LINELEN];
extern char jwt_token[LINELEN];

int is_logged_in();
int has_lib_access();
int init_connection(char *name, uint16_t port);
void run_server();

#endif