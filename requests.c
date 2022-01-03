#include "requests.h"

void compute_message(char *message, char *line) {
	strcat(message, line);
	strcat(message, CRLF);
}

char *compute_get_request(char *host, char *url,
							char *token,
							char *cookie) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));

	// write the method name, URL and protocol type
	sprintf(line, "GET %s HTTP/1.1", url);

	compute_message(message, line);

	// add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// add additional headers
	if (token && strlen(token) != 0) {
		sprintf(line, "Authorization: Bearer %s", token);

		compute_message(message, line);
	}

	// add cookies, according to the protocol format
	if (cookie && strlen(cookie) != 0) {
		sprintf(line, "Cookie: %s", cookie);

		compute_message(message, line);
	}

	// add final new line
	compute_message(message, "");

	free(line);

	return message;
}

char *compute_post_request(char *host, char *url,
							char *content_type,
							char *token,
							char *cookie,
							char *body_data) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));

	// write the method name, URL and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);
	
	// add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// add additional headers
	if (token && strlen(token) != 0) {
		sprintf(line, "Authorization: Bearer %s", token);

		compute_message(message, line);
	}

	// add necessary headers (Content-Type and Content-Length are mandatory)
	// in order to write Content-Length you must first compute the message size)
	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);

	sprintf(line, "Content-Length: %ld", strlen(body_data));
	compute_message(message, line);

	// add cookies (optional)
	if (cookie && strlen(cookie) != 0) {
		sprintf(line, "Cookie: %s", cookie);

		compute_message(message, line);
	}
	// add new line at end of header
	compute_message(message, "");


	// add the actual payload data
	compute_message(message, body_data);
	compute_message(message, "");

	free(line);

	return message;
}

char *compute_delete_request(char *host, char *url,
								char *token,
								char *cookie) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));

	// write the method name, URL and protocol type
	sprintf(line, "DELETE %s HTTP/1.1", url);

	compute_message(message, line);

	// add the host
	sprintf(line, "Host: %s", host);
	compute_message(message, line);

	// add additional headers
	if (token && strlen(token) != 0) {
		sprintf(line, "Authorization: Bearer %s", token);

		compute_message(message, line);
	}

	// add cookies, according to the protocol format
	if (cookie && strlen(cookie) != 0) {
		sprintf(line, "Cookie: %s", cookie);

		compute_message(message, line);
	}

	// add final new line
	compute_message(message, "");

	free(line);

	return message;
}

void send_to_server(char *message) {
	int max_length = strlen(message);
	int bytes_sent = 0;

	// write as much as we can at a time
	while (bytes_sent < max_length) {
		char *start_at = message + bytes_sent;
		int bytes_remaining = max_length - bytes_sent;

		int result = write(sockfd, start_at, bytes_remaining);
		DIE(result < 0, "[client] ERROR: Could not write to "
						"socket '%d'\n", sockfd);

		if (result == 0) {
			// no more writing
			break;
		}

		// increment
		bytes_sent += result;
	}
}

char *receive_from_server() {
	char response[BUFLEN] = {0};
	buffer buffer = buffer_init();

	int header_end = 0;
	int content_len = 0;

	while (TRUE) {
		// read from socket
		int result = read(sockfd, response, BUFLEN);
		DIE(result < 0, "[client] ERROR: Could not read header from "
						"socket '%d'\n", sockfd);

		if (result == 0) {
			// stop reading
			break;
		}

		buffer_add(&buffer, response, (size_t) result);

		header_end = buffer_find(&buffer, HEADER_TERMINATOR,
									HEADER_TERMINATOR_SIZE);

		if (header_end >= 0) {
			header_end += HEADER_TERMINATOR_SIZE;

			int content_len_start = buffer_find_insensitive(&buffer,
														CONTENT_LENGTH,
														CONTENT_LENGTH_SIZE);

			if (content_len_start < 0) {
				continue;
			}

			content_len_start += CONTENT_LENGTH_SIZE;
			content_len = strtol(buffer.data + content_len_start, NULL, 10);
			break;
		}
	}

	size_t total = content_len + (size_t) header_end;

	while (buffer.size < total) {
		int result = read(sockfd, response, BUFLEN);
		DIE(result < 0, "[client] ERROR: Could not read message body from "
						"socket '%d'\n", sockfd);

		if (result == 0) {
			// stop reading
			break;
		}

		buffer_add(&buffer, response, (size_t) result);
	}

	buffer_add(&buffer, "", 1);

	return buffer.data;
}