#ifndef _PROMPTS_H_
#define _PROMPTS_H_

#include "helpers.h"
#include "parson.h"
#include "commands.h"

// for books_prompt 'mode'
#define BOOKS_ALL			0
#define BOOKS_SINGLE		1

int userpass_prompt(JSON_Value **value);
int books_prompt(int mode, char *path);
int add_book_prompt(JSON_Value **value);

#endif