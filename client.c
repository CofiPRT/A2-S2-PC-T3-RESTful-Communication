#include "client.h"

char login_cookie[LINELEN] = {0};
char jwt_token[LINELEN] = {0};

int is_logged_in() {
	return strlen(login_cookie) != 0;
}

int has_lib_access() {
	return strlen(jwt_token) != 0;
}

int init_connection(char *name, uint16_t port) {
	// get an address by host name
	struct addrinfo hints;
	struct addrinfo *addr;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;

	int result = getaddrinfo(name, NULL, &hints, &addr);
	DIE(result != 0, "[client] ERROR: Could not get address info for "
					"address '%s'\n", name);
	GOOD("[client] INFO: Retrieved address for '%s'\n", name);


	// open the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	DIE(sockfd < 0, "[client] ERROR: Could not open socket\n");
	GOOD("[client] INFO: Opened socket at '%d'\n", sockfd);


	// fill in the server struct
	struct sockaddr_in serv_addr;
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	// cast to a useful struct
	struct sockaddr_in *useful_addr = (struct sockaddr_in *) (addr->ai_addr);
	// retrieve the IP address
	size_t length = sizeof(serv_addr.sin_addr);
	memcpy(&(serv_addr.sin_addr), &(useful_addr->sin_addr), length);


	// attempt connection
	struct sockaddr *cast_addr = (struct sockaddr *) &serv_addr;
	length = sizeof(serv_addr);

	result = connect(sockfd, cast_addr, length);
	DIE(result < 0, "[client] ERROR: Could not connect to "
					"socket '%d'\n", sockfd);
	GOOD("[client] INFO: Connected to socket '%d'\n", sockfd);

	return sockfd;
}

void run_client() {
	// function mappings
	FMap map[] = {
		{"register", 		command_register},
		{"login",			command_login},
		{"enter_library",	command_enter_library},
		{"get_books",		command_get_books},
		{"get_book",		command_get_single_book},
		{"add_book",		command_add_book},
		{"delete_book",		command_delete_book},
		{"logout",			command_logout}
	};
	int f_number = sizeof(map) / sizeof(FMap);

	// keep waiting for input until "exit" is issued
	while (TRUE) {
		char buffer[BUFLEN] = {0};

		// read from STDIN
		if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
			fprintf(stderr, "[client] ERROR: Could not read from STDIN\n");
			continue;
		}

		GOOD("[client] INFO: Read command from STDIN\n");

		// parse command
		char command[BUFLEN] = {0};

		int result = sscanf(buffer, "%s %*s ", command);

		if (result != 1) {
			fprintf(stderr, "[client] ERROR: Invalid number of arguments!\n"
						"\tExpected: 1 (the command)\n"
						"\tReceived: '%d'\n", result);

			continue; // try again
		}

		if (!strcmp(command, "exit")) {
			// stop the client
			break;
		}

		int command_found = FALSE;

		for (int i = 0; i < f_number; i++) {
			// find command
			if (!strcmp(map[i].fname, command)) {
				command_found = TRUE;

				map[i].f(sockfd);
				break;
			}
		}

		if (!command_found) {
			fprintf(stderr, "[client] ERROR: Command '%s' not found! "
							"Please enter a valid command:\n"
							"\tregister\n"
							"\tlogin\n"
							"\tenter_library\n"
							"\tget_books\n"
							"\tget_book\n"
							"\tadd_book\n"
							"\tdelete_book\n"
							"\tlogout\n"
							"\texit\n", command);
		}
	}
}

int main() {
	// macros defined in 'helpers.h'
	sockfd = init_connection(HOST_NAME, PORT);

	// do client stuff
	run_client();

	// close this connection
	close(sockfd);

	return 0;
}