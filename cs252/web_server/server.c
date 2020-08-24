#include "server.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "http_messages.h"
#include "routes.h"

#define BUF_SIZE (1024)
#define SUCCESS (1)
#define PARSE_ERROR (-1)
#define CRLF ("\r\n")

int parse_request(http_request *, socket_t *);
char *substring(char *, int, int);

char g_user_pass[MAX_USR_PWD_LEN];

/*
 * Return a string in a format <user>:<password>
 * either from auth.txt or from your implememtation.
 */

char *return_user_pwd_string(void) {
  // Read from ./auth.txt. Don't change this. We will use it for testing
  FILE *fp = NULL;
  char *line = (char *)malloc(sizeof(char) * MAX_USR_PWD_LEN);
  size_t len = 0;

  fp = fopen("./auth.txt", "r");
  if (fp == NULL) {
    perror("couldn't read auth.txt");
    exit(-1);
  }

  if (getline(&line, &len, fp) == -1) {
    perror("couldn't read auth.txt");
    exit(-1);
  }

  sprintf(g_user_pass, "%s", line);

  free(line);

  return g_user_pass;
} /* return_user_pwd_string() */

/*
 * Accept connections one at a time and handle them.
 */

void run_linear_server(acceptor *acceptor) {
  while (1) {
    socket_t *sock = accept_connection(acceptor);
    handle(sock);
  }
} /* run_linear_server() */

/*
 * Accept connections, creating a different child process to handle each one.
 */

void run_forking_server(acceptor *acceptor) {
  // TODO: Add your code to accept and handle connections in child processes
} /* run_forking_server() */

/*
 * Accept connections, creating a new thread to handle each one.
 */

void run_threaded_server(acceptor *acceptor) {
  // TODO: Add your code to accept and handle connections in new threads
} /* run_threaded_server() */

/*
 * Accept connections, drawing from a thread pool with num_threads to handle the
 * connections.
 */

void run_thread_pool_server(acceptor *acceptor, int num_threads) {
  // TODO: Add your code to accept and handle connections in threads from a
  // thread pool
} /* run_thread_pool_server() */


/*
 * This function is used to find a substring of a buf.
 */

char *substring(char *buf, int start, int end) {
  char *new_str = (char *) malloc(end - start + 1);
  for (int i = 0; i < end - start; i++) {
    new_str[i] = buf[start + i];
  }
  new_str[end - start] = '\0';
  return new_str;
} /* substring */


/*
 * This fuction is used to parse an individual header.
 */

header *parse_header(char *head) {

  char *space = strchr(head, ' ');
  if (space == NULL) {
    //printf("Invalid Header\n");
    return NULL;
  }

  header *headr = (header *) malloc(sizeof(header));

  headr->key = substring(head, 0, space - head - 1);
  space++;
  headr->value = substring(space, 0, strlen(space));

  //printf("Key: {%s}\n", headr->key);
  //printf("Val: {%s}\n", headr->value);

  return headr;

} /* parse_header() */


int parse_all_headers(http_request *request, socket_t *sock, char *buf) {

  /* This is the location of the next CRLF in the headers */

  char *crlf = strstr(buf, CRLF);
  
  /* This is the string containing all the un-parsed headers */

  char *rest_headers = crlf + strlen(CRLF);
  
  /* Find the location of the next CRLF */

  crlf = strstr(rest_headers, CRLF);

  while (crlf != NULL) {
    /* Extract the header */

    char *headr = substring(rest_headers, 0, crlf - rest_headers);

    header *head = parse_header(headr);
    free(headr);
    headr = NULL;
    if (head == NULL) {
      return PARSE_ERROR;
    }

    /* Update the request */

    request->num_headers++;
    request->headers = realloc(request->headers,
      request->num_headers * sizeof(header));
    request->headers[request->num_headers - 1] = *head;

    /* Update rest_headers and crlf */

    rest_headers = crlf + strlen(CRLF);
    crlf = strstr(rest_headers, CRLF);

    /* Break the loop if the last crlf and next crlf are next to each other */
    
    if (rest_headers ==  crlf) {
      break;
    }
  }

  return SUCCESS;
}

/*
 * This function is used to parse an incoming http request.
 */

int parse_request(http_request *request, socket_t *sock) {
  /* Ensure that the request is filled with default values */
  request->method = "method";
  request->request_uri = "uri";
  request->http_version = "http_version";
  request->num_headers = 0;;
  request->query = "query";
  request->message_body = "message body";

  /* Buffer to hold the contents of the socket */

  char *buf = (char *) malloc(BUF_SIZE);

  /* Read the contents of sock into BUF.
   * Try reading BUF_SIZE characters */

  int check = socket_read(sock, buf, BUF_SIZE);
  if (check == -1) {

    /* Error */

    fprintf(stderr, "error\n");
  }
  else {
    printf("Contents of Socket:\n----------------\n%s\n----------------\n", buf);
  }


  /* Parse the method */
  /* Pointer to the first space in buffer */

  char *space = strchr(buf, ' ');
  char *method = NULL;
  if (space != NULL) {
    method = substring(buf, 0, space - buf);
    request->method = strdup(method);
    free(method);
    method = NULL;
  }
  else {
    return PARSE_ERROR;
  }

  /* Parse the request uri */
  /* Pointer to the first character of the request uri */

  char *request_uri = space + 1;
  space = strchr(request_uri, ' ');
  char *uri = NULL;
  if (space != NULL) {
    uri = substring(request_uri, 0, space - request_uri);
    request->request_uri = strdup(uri);
    free(uri);
    uri = NULL;
  }
  else {
    return PARSE_ERROR;
  }

  /* Parse the http version */
  /* Pointer to the first character of the version */

  char *version = space + 1;
  space = strstr(version, "\r\n");
  char *http_version = NULL;
  if (space != NULL) {
    http_version = substring(version, 0, space - version);
    request->http_version = strdup(http_version);
    free(http_version);
    http_version = NULL;
  }
  else {
    return PARSE_ERROR;
  }

  return parse_all_headers(request, sock, buf);
} /* parse_request() */

/*
 * Handle an incoming connection on the passed socket.
 */

void handle(socket_t *sock) {
  http_request request = {0};

  // PRIORITY 1
  // TODO: Replace this code and actually parse the HTTP request

  int value = parse_request(&request, sock);
  if (value == PARSE_ERROR) {
    fprintf(stderr, "Error parsing the request\n");
  }
  print_request(&request);

  http_response response = {0};

  // PRIORITY 2
  // TODO: Add your code to create the correct HTTP response

  handle_htdocs(&request);

  response.http_version = "HTTP/1.1";

  char *to_string = response_string(&response);
  printf("%s\n", to_string);
  socket_write_string(sock, to_string);

  free(to_string);
  to_string = NULL;

  close_socket(sock);
} /* handle() */
