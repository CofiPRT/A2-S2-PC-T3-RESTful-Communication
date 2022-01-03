#ifndef _REQUESTS_
#define _REQUESTS_

#include "helpers.h"
#include "buffer.h"

void compute_message(char *message, char *line);

// computes and returns a GET request string
// cookies can be set to NULL if not needed
// additional header can be set to NULL if not needed
char *compute_get_request(char *host, char *url,
							char *token,
							char *cookie);

// computes and returns a POST request string
// cookies can be set to NULL if not needed
// additional header can be set to NULL if not needed
char *compute_post_request(char *host, char *url,
							char *content_type,
							char *token,
							char *cookie,
							char *body_data);

// computes and returns a DELETE request string
// cookies can be set to NULL if not needed
// additional header can be set to NULL if not needed
char *compute_delete_request(char *host, char *url,
								char *token,
								char *cookie);

void send_to_server(char *buffer);
char *receive_from_server();

#endif