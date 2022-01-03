#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

// reading message from server
#define HEADER_TERMINATOR 		"\r\n\r\n"
#define HEADER_TERMINATOR_SIZE 	(sizeof(HEADER_TERMINATOR) - 1)
#define CONTENT_LENGTH 			"Content-Length: "
#define CONTENT_LENGTH_SIZE 	(sizeof(CONTENT_LENGTH) - 1)
#define CRLF					"\r\n"
#define WHITESPACES				" \f\n\r\t\v"

// buffer lengths
#define BUFLEN 					4096
#define LINELEN 				1000
#define SMALL_LEN				100

// connection parameters
#define HOST_NAME 			"ec2-3-8-116-10.eu-west-2.compute.amazonaws.com"
#define PORT				8080

#define PATH_REGISTER		"/api/v1/tema/auth/register"
#define PATH_LOGIN			"/api/v1/tema/auth/login"
#define PATH_LOGOUT			"/api/v1/tema/auth/logout"
#define PATH_LIB_ACCESS		"/api/v1/tema/library/access"
#define PATH_LIB_BOOKS		"/api/v1/tema/library/books"

#define CONTENT_JSON		"application/json"

// global usage
#define TRUE				1
#define FALSE				0

// TOGGLEABLE MACROS

// logging, see GOOD macro below
#define LOG					FALSE

// if condition -> display message AND exit program
#define DIE(assertion, fmt, ...)						\
	do {												\
		if (assertion) {								\
			fprintf(stderr, "(%s, %d): " fmt,			\
					__FILE__, __LINE__, ## __VA_ARGS__);\
			exit(EXIT_FAILURE);							\
		}												\
	} while(FALSE)

// if logging is enabled, log the message to stdout
#define GOOD(fmt, ...)									\
	do {												\
		if (LOG) {										\
			printf("(%s, %d): " fmt,					\
					__FILE__, __LINE__, ## __VA_ARGS__);\
		}												\
	} while(FALSE)

extern int sockfd;

char *trim(char *string);

#endif