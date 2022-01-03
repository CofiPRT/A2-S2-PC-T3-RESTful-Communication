#include "commands.h"

int command_register() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (userpass_post(PATH_REGISTER, &response) == COMMAND_SUCCESS &&
		check_code(response, GOOD_REGISTER) 	== COMMAND_SUCCESS) {

		status = COMMAND_SUCCESS;
		printf("[client] INFO: Registration successful\n");
	}	

	free(response);
	return status;
}

int command_login() {
	if (is_logged_in()) {
		fprintf(stderr, "[client] ERROR: Already logged in!\n");
		return COMMAND_ERROR;
	}

	int status = COMMAND_ERROR;

	char *response = NULL;

	if (userpass_post(PATH_LOGIN, &response) 	== COMMAND_SUCCESS &&
		check_code(response, GOOD_LOGIN) 		== COMMAND_SUCCESS &&
		extract_login_cookie(response) 			== COMMAND_SUCCESS) {
		
		status = COMMAND_SUCCESS;
		printf("[client] INFO: Login successful\n");
	}

	free(response);
	return status;
}

int command_enter_library() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (simple_get(PATH_LIB_ACCESS, &response) 		== COMMAND_SUCCESS &&
		check_code(response, GOOD_ENTER_LIBRARY) 	== COMMAND_SUCCESS &&
		extract_jwt_token(response) 				== COMMAND_SUCCESS) {

		status = COMMAND_SUCCESS;
		printf("[client] INFO: Library access granted\n");
	}

	free(response);
	return status;
}

int command_get_books() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (books_get(BOOKS_ALL, &response) 		== COMMAND_SUCCESS &&
		check_code(response, GOOD_GET_BOOKS) 	== COMMAND_SUCCESS &&
		print_JSON(response, BOOKS_ALL) 		== COMMAND_SUCCESS) {
		
		status = COMMAND_SUCCESS;
	}

	free(response);
	return status;
}

int command_get_single_book() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (books_get(BOOKS_SINGLE, &response) 	== COMMAND_SUCCESS &&
		check_code(response, GOOD_GET_BOOK) == COMMAND_SUCCESS &&
		print_JSON(response, BOOKS_SINGLE) 	== COMMAND_SUCCESS) {

		status = COMMAND_SUCCESS;
	}

	free(response);
	return status;
}

int command_add_book() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (book_post(&response) 				== COMMAND_SUCCESS &&
		check_code(response, GOOD_ADD_BOOK) == COMMAND_SUCCESS) {
		
		status = COMMAND_SUCCESS;

		printf("[client] INFO: Book successfully added\n");
	}

	free(response);
	return status;
}

int command_delete_book() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (book_delete(&response) 					== COMMAND_SUCCESS &&
		check_code(response, GOOD_DELETE_BOOK) 	== COMMAND_SUCCESS) {

		status = COMMAND_SUCCESS;

		printf("[client] INFO: Book successfully deleted\n");
	}

	free(response);

	return status;
}

int command_logout() {
	int status = COMMAND_ERROR;

	char *response = NULL;

	if (simple_get(PATH_LOGOUT, &response) == COMMAND_SUCCESS &&
		check_code(response, GOOD_LOGOUT) == COMMAND_SUCCESS) {

		status = COMMAND_SUCCESS;

		// reset credentials
		memset(login_cookie, 0, sizeof(login_cookie));
		memset(jwt_token, 0, sizeof(jwt_token));

		printf("[client] INFO: Logout successful\n");
	}

	free(response);

	return status;
}
