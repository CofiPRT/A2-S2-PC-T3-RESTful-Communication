#include "prompts.h"

int userpass_prompt(JSON_Value **value) {
	// initialise the JSON
	*value = json_value_init_object();
	JSON_Object *object = json_value_get_object(*value);

	char input[LINELEN] = {0};
	char *parsed_input;

	// prompt user for username
	memset(input, 0, sizeof(input));
	printf("username=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read username\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "username", parsed_input);

	// prompt user for password
	memset(input, 0, sizeof(input));
	printf("password=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read password\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "password", parsed_input);

	return COMMAND_SUCCESS;
}

int books_prompt(int mode, char *path) {
	switch (mode) {
		case BOOKS_ALL: {
			// normal path
			sprintf(path, "%s", PATH_LIB_BOOKS);

			break;
		}

		case BOOKS_SINGLE: {
			// prompt user for the book ID
			char id[LINELEN] = {0};

			printf("id=");
			char *parsed_id = trim(fgets(id, sizeof(id), stdin));

			if (parsed_id == NULL) {
				fprintf(stderr, "[client] ERROR: Could not read id\n");
				return COMMAND_ERROR;
			}

			sprintf(path, "%s/%s", PATH_LIB_BOOKS, parsed_id);

			break;
		}

		default: {
			// should not reach here
			fprintf(stderr, "[client] ERROR: Invalid mode for books GET "
							"request\n");
			return COMMAND_ERROR;
		}
	}

	return COMMAND_SUCCESS;
}

int add_book_prompt(JSON_Value **value) {
	// initialize the JSON
	*value = json_value_init_object();
	JSON_Object *object = json_value_get_object(*value);

	// prompt user for details
	char input[LINELEN] = {0};
	char *parsed_input;

	// TITLE
	memset(input, 0, sizeof(input));
	printf("title=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read title\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "title", parsed_input);

	// AUTHOR
	memset(input, 0, sizeof(input));
	printf("author=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read author\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "author", parsed_input);

	// GENRE
	memset(input, 0, sizeof(input));
	printf("genre=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read genre\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "genre", parsed_input);

	// PUBLISHER
	memset(input, 0, sizeof(input));
	printf("publisher=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read publisher\n");
		return COMMAND_ERROR;
	}

	json_object_set_string(object, "publisher", parsed_input);

	// PAGE COUNT
	memset(input, 0, sizeof(input));
	printf("page_count=");
	parsed_input = trim(fgets(input, sizeof(input), stdin));

	if (parsed_input == NULL) {
		fprintf(stderr, "[client] ERROR: Could not read page count\n");
		return COMMAND_ERROR;
	}

	int page_count = atoi(input);
	if (page_count <= 0) {
		fprintf(stderr, "[client] ERROR: Invalid page count. "
						"Please attempt to add the book again, "
						"and enter a strictly positive page count\n");
		return COMMAND_ERROR;
	}
	json_object_set_number(object, "page_count", page_count);

	return COMMAND_SUCCESS;
}