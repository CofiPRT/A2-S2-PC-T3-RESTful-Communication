#include "commands_helpers.h"

int get_error(char *response, char *error_msg) {
	if (response && response[0] == '\0') {
		// no error
		return COMMAND_ERROR;
	}

	// jump to the data
	char *response_data = strstr(response, "\r\n\r\n") + 4;

	JSON_Value *value = json_parse_string(response_data);

	if (value == NULL) {
		// no JSON, just plain text
		sprintf(error_msg, "%s", response_data);
		json_value_free(value);
		return COMMAND_SUCCESS;
	}

	JSON_Object *object = json_value_get_object(value);

	// extract the error message
	const char *json_string = json_object_get_string(object, "error");

	if (!json_string) {
		fprintf(stderr, "[client] ERROR: Could not extract error message. "
						"Pretty ironic.\n");
		json_value_free(value);
		return COMMAND_ERROR;
	}

	sprintf(error_msg, "%s", json_string);

	json_value_free(value);
	
	return COMMAND_SUCCESS;
}

int check_code(char *response, char *code) {
	char r_code[SMALL_LEN] = {0};

	int result = sscanf(response, "%*s %s ", r_code);

	if (result == 0) {
		fprintf(stderr, "[client] ERROR: Could not parse response code\n");
		return COMMAND_ERROR;
	}

	if (strcmp(r_code, code)) {
		char error_msg[LINELEN] = {0};

		if (get_error(response, error_msg) == COMMAND_ERROR) {
			return COMMAND_ERROR;
		}

		fprintf(stderr, "[server] ERROR: %s\n", error_msg);

		return COMMAND_ERROR;
	}

	GOOD("[client] INFO: Request code matches the expected code %s\n", code);

	return COMMAND_SUCCESS;
}

int attempt_send(char *message, char **response) {
	int attempts = 0;

	// attempt to send message a maximum number of times
	while (attempts < MAX_ATTEMPTS) {
		attempts++;

		send_to_server(message);
		GOOD("[client] INFO: Sent message:\n%s\n", message);

		*response = receive_from_server();

		// the message is empty because... can't answer this

		if (strlen(*response) == 0) {
			// attempt reconnection
			GOOD("[client] INFO: Message is empty, attempting reconnection\n");
			free(*response);

			close(sockfd);
			sockfd = init_connection(HOST_NAME, PORT);

		} else {
			GOOD("[client] INFO: Received message:\n%s\n", *response);
			return COMMAND_SUCCESS;
		}
	}

	fprintf(stderr, "[client] ERROR: Could not reconnect. "
					"Maximum number of attempts reached\n");

	return COMMAND_ERROR;
}
