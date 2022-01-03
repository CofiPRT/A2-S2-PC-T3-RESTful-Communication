#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include "helpers.h"
#include "commands_requests.h"
#include "commands_helpers.h"
#include "commands_follow.h"
#include "prompts.h"

#define COMMAND_SUCCESS		1
#define COMMAND_ERROR		0

#define GOOD_REGISTER		"201"
#define GOOD_LOGIN			"200"
#define GOOD_ENTER_LIBRARY	"200"
#define GOOD_GET_BOOKS		"200"
#define GOOD_GET_BOOK		"200"
#define GOOD_ADD_BOOK		"200"
#define GOOD_DELETE_BOOK	"200"
#define GOOD_LOGOUT			"200"

typedef struct function_map {
	char *fname; // function name
	int (*f)(); // function pointer
} FMap;

int command_register();
int command_login();
int command_enter_library();
int command_get_books();
int command_get_single_book();
int command_add_book();
int command_delete_book();
int command_logout();

#endif