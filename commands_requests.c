#include "commands_requests.h"

int userpass_post(char *path, char **response) {
	int status = COMMAND_ERROR;

	JSON_Value *value = NULL;

	char *message = NULL;

	// construct the POST
	if (userpass_prompt(&value) == COMMAND_SUCCESS) {
		// create the message
		message = compute_post_request(HOST_NAME, path, CONTENT_JSON,
							jwt_token, login_cookie,
							json_serialize_to_string_pretty(value));

		GOOD("[client] INFO: Created POST request containing "
				"username and password\n");

		if (attempt_send(message, response) == COMMAND_SUCCESS) {
			status = COMMAND_SUCCESS; // only one way to success

			GOOD("[client] INFO: POST request sent\n");
		}
	}

	json_value_free(value);
	free(message);

	return status;
}

int simple_get(char *path, char **response) {
	int status = COMMAND_ERROR;

	// construct the GET
	// create the message
	char *message = compute_get_request(HOST_NAME, path,
						jwt_token, login_cookie);

	GOOD("[client] INFO: Created GET request simply containing "
			"the session ID and library authorization\n");

	if (attempt_send(message, response) == COMMAND_SUCCESS) {
		status = COMMAND_SUCCESS; // only one way to success

		GOOD("[client] INFO: GET request sent\n");
	}

	free(message);

	return status;
}

int books_get(int mode, char **response) {
	int status = COMMAND_ERROR;

	char path[LINELEN] = {0};

	char *message = NULL;
	
	// construct the GET
	if (books_prompt(mode, path) == COMMAND_SUCCESS) {
		// create the message
		message = compute_get_request(HOST_NAME, path,
					jwt_token, login_cookie);

		GOOD("[client] INFO: Created GET request for one or multiple books\n");

		if (attempt_send(message, response) == COMMAND_SUCCESS) {
			status = COMMAND_SUCCESS; // only one way to success

			GOOD("[client] INFO: GET request sent\n");
		}
	}

	free(message);

	return status;
}

int book_post(char **response) {
	int status = COMMAND_ERROR;

	JSON_Value *value = NULL;
	char *message = NULL;

	// construct the POST
	if (add_book_prompt(&value) == COMMAND_SUCCESS) {
		// create the message
		char *message = compute_post_request(HOST_NAME,
							PATH_LIB_BOOKS, CONTENT_JSON,
							jwt_token,
							login_cookie,
							json_serialize_to_string_pretty(value));

		GOOD("[client] INFO: Created POST request containing a new book\n");

		if (attempt_send(message, response) == COMMAND_SUCCESS) {
			status = COMMAND_SUCCESS; // only one way to success

			GOOD("[client] INFO: POST request sent\n");
		}
	}

	json_value_free(value);
	free(message);

	return status;
}

int book_delete(char **response) {
	int status = COMMAND_ERROR;

	char path[LINELEN] = {0};

	char *message = NULL;

	// construct the DELETE
	if (books_prompt(BOOKS_SINGLE, path) == COMMAND_SUCCESS) {
		// create the message
		message = compute_delete_request(HOST_NAME, path,
					jwt_token, login_cookie);

		GOOD("[client] INFO: Created DELETE request for a specific book\n");

		if (attempt_send(message, response) == COMMAND_SUCCESS) {
			status = COMMAND_SUCCESS; // only one way to success

			GOOD("[client] INFO: DELETE request sent\n");
		}
	}

	free(message);

	return status;
}