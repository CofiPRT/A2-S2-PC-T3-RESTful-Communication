#include "commands_follow.h"

int extract_login_cookie(char *response) {
	// compute the limits of the cookie
	char *start = strstr(response, "Set-Cookie: ") + strlen("Set-Cookie: ");
	if (!start) {
		fprintf(stderr, "[client] ERROR: Could not find cookie start\n");
		return COMMAND_ERROR;
	}

	char *end = strstr(start, "\r\n");
	if (!end) {
		fprintf(stderr, "[client] ERROR: Could not find cookie end\n");
		return COMMAND_ERROR;
	}

	// extract
	strncpy(login_cookie, start, end - start);

	GOOD("[CLIENT] INFO: Extracted login cookie:\n%s\n", login_cookie);

	return COMMAND_SUCCESS;
}

int extract_jwt_token(char *response) {
	if (response && response[0] == '\0') {
		fprintf(stderr, "[client] ERROR: Could not extract the JWT token. "
						"Invalid message\n");
		return COMMAND_ERROR;
	}

	// jump to the data
	char *response_data = strstr(response, "\r\n\r\n") + 4;

	JSON_Value *value = json_parse_string(response_data);
	JSON_Object *object = json_value_get_object(value);

	// extract the token
	const char *token = json_object_get_string(object, "token");

	if (!token) {
		fprintf(stderr, "[client] ERROR: Unexpected JWT token format\n");
		json_value_free(value);
		return COMMAND_ERROR;
	}

	sprintf(jwt_token, "%s", token);

	json_value_free(value);

	GOOD("[client] INFO: Extracted JWT token:\n%s\n", jwt_token);
	return COMMAND_SUCCESS;
}

int print_JSON(char *response, int mode) {
	if (response && response[0] == '\0') {
		fprintf(stderr, "[client] ERROR: Could not extract JSON. "
						"Invalid message\n");
		return COMMAND_ERROR;
	}

	// jump to the data
	char *response_data = strstr(response, "\r\n\r\n") + 4;

	if (!response_data) {
		fprintf(stderr, "[client] ERROR: Could not find JSON. "
						"Invalid message\n");
		return COMMAND_ERROR;
	}

	// extract the JSON
	JSON_Value *value = json_parse_string(response_data);
	JSON_Array *array = json_value_get_array(value);
	
	if (mode == BOOKS_ALL) {
		printf("Available books:\n");

		int books_count = json_array_get_count(array);

		if (books_count == 0) {
			// show this fact
			printf("\tNone\n");
		}

		// for every book
		for (int i = 0; i < books_count; i++) {
			JSON_Object *object = json_array_get_object(array, i);
			printf("\tID: %d\n"
					"\tTitle: %s\n"
					"\t----------\n"
					"\t\n",
					(int)json_object_get_number(object, "id"),
					json_object_get_string(object, "title"));
		}

	} else if (mode == BOOKS_SINGLE) {
		JSON_Object *object = json_array_get_object(array, 0); // first
		printf("Requested book:\n"
				"\tTitle: %s\n"
				"\tAuthor: %s\n"
				"\tPublisher: %s\n"
				"\tGenre: %s\n"
				"\tPage count: %d\n",
				json_object_get_string(object, "title"),
				json_object_get_string(object, "author"),
				json_object_get_string(object, "publisher"),
				json_object_get_string(object, "genre"),
				(int)json_object_get_number(object, "page_count"));
	} else {
		fprintf(stderr, "[client] ERROR: Invalid mode for books print "
						"format\n");
		json_value_free(value);
		return COMMAND_ERROR;
	}

	json_value_free(value);
	return COMMAND_SUCCESS;
}